/*
 * A191743  Smallest numbers with a given factorization pattern in their
 *          sequence of divisors.
 * https://oeis.org/A191743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 3011, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fnv1a_64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A191743 – Smallest numbers with a given factorization pattern of their divisors.
   a(n) returns the nth member (offset 1).
   The algorithm scans integers, computes the multiset of exponent tuples
   of all divisors, hashes that tuple sequence, and keeps the smallest
   integer with each new hash.  Results are memoised so calls may come
   in any order.                                       */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/fnv1a_64.h"

/* ------------------------------------------------------------------ */
/* --- Static storage ------------------------------------------------ */
static idx_t  seq_len      = 0;       /* number of terms stored (offset 1) */
static size_t seq_cap      = 0;       /* capacity of seq_arr              */
static seq_t *seq_arr      = NULL;    /* array of computed terms          */

static uint64_t last_scanned = 0;     /* last integer examined          */
static size_t   pattern_ct   = 0;     /* number of unique patterns seen */

/* ------------------------------------------------------------------ */
/* --- Hash table for pattern fingerprints -------------------------- */
#define HT_INIT_SIZE 16384              /* power of two */
static uint64_t *ht_table  = NULL;     /* 0 == empty slot (fingerprint+1 stored) */
static size_t   ht_mask    = HT_INIT_SIZE - 1;

/* ------------------------------------------------------------------ */
/* --- Prime list for trial division -------------------------------- */
#define PRIME_LIMIT 2000000
static uint32_t *prime_list = NULL;
static size_t   prime_cnt   = 0;

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* --- Ensure hash table capacity ----------------------------------- */
static void ht_expand(void)
{
    size_t new_size = (ht_mask + 1) * 2;
    uint64_t *new_tbl = (uint64_t*)calloc(new_size, sizeof(uint64_t));
    if (!new_tbl) abort();

    size_t new_mask = new_size - 1;
    for (size_t i = 0; i <= ht_mask; ++i) {
        uint64_t val = ht_table[i];
        if (val) {
            /* simple secondary hashing: use xor‑shift */
            size_t h = (val ^ (val >> 33u)) & new_mask;
            while (new_tbl[h]) h = (h + 1) & new_mask;
            new_tbl[h] = val;
        }
    }
    free(ht_table);
    ht_table = new_tbl;
    ht_mask  = new_mask;
}

static void ht_init(void)
{
    if (ht_table) return;
    ht_table = (uint64_t*)calloc(HT_INIT_SIZE, sizeof(uint64_t));
    if (!ht_table) abort();
    ht_mask  = HT_INIT_SIZE - 1;
}

/* ------------------------------------------------------------------ */
/* --- Prime sieve (once) ------------------------------------------ */
static void primes_init(void)
{
    if (prime_list) return;

    bool *is_prime = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime) abort();
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = true;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;

    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    prime_list = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) abort();
    prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) prime_list[prime_cnt++] = (uint32_t)i;

    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* ----- Divisor generation ------------------------------------------ */
#define MAX_P 12                  /* maximum distinct prime factors (~12 for n<1e12) */
#define MAX_D 8192                /* maximum divisor count we expect */

typedef struct {
    uint64_t val;
    uint8_t exps[MAX_P];
} Div;

static Div    div_buf[MAX_D];
static uint8_t cur_exps[MAX_P];
static size_t div_cnt;

/* recursive divisor generation */
static void gen_divisors(const uint64_t *primes,
                         const uint8_t *exps,
                         size_t k,
                         size_t pos,
                         uint64_t val)
{
    if (pos == k) {
        div_buf[div_cnt].val = val;
        memcpy(div_buf[div_cnt].exps, cur_exps, k);
        ++div_cnt;
        return;
    }
    uint64_t v = val;
    for (uint8_t e = 0; e <= exps[pos]; ++e) {
        cur_exps[pos] = e;
        gen_divisors(primes, exps, k, pos + 1, v);
        v *= primes[pos];
    }
}

/* comparison for qsort (ascending divisor value) */
static int cmp_div(const void *a, const void *b)
{
    const Div *da = (const Div*)a;
    const Div *db = (const Div*)b;
    if (da->val < db->val) return -1;
    if (da->val > db->val) return 1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* ----- Factorization (trial division) ------------------------------ */
static void factorize(uint64_t n,
                      uint64_t *primes,
                      uint8_t *exps,
                      size_t *k)
{
    size_t idx = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        uint32_t p = prime_list[i];
        if (temp % p == 0) {
            uint8_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[idx] = p;
            exps[idx]   = e;
            ++idx;
        }
    }
    if (temp > 1) {
        primes[idx] = temp;
        exps[idx]   = 1;
        ++idx;
    }
    *k = idx;
}

/* ------------------------------------------------------------------ */
/* ----- Check/Add new pattern -------------------------------------- */
static bool pattern_is_new(const uint8_t *exps,
                           size_t k,
                           const Div *divs,
                           size_t dcount)
{
    /* Compute hash of the ordered sequence of exponent tuples */
    uint64_t h = 14695981039346656037ULL;              /* FNV offset basis */
    const uint64_t FNV_PRIME = 1099511628211ULL;       /* FNV prime */

    for (size_t i = 0; i < dcount; ++i) {
        for (size_t j = 0; j < k; ++j) {
            h ^= (uint64_t)divs[i].exps[j];
            h *= FNV_PRIME;
        }
        /* delimiter between divisors */
        h ^= 0xFF;
        h *= FNV_PRIME;
    }

    /* Convert to value non‑zero to use 0 as empty slot */
    h += 1;
    size_t idx = h & ht_mask;
    while (ht_table[idx]) {
        if (ht_table[idx] == h) return false;    /* already seen */
        idx = (idx + 1) & ht_mask;
    }
    /* new pattern, insert */
    ht_table[idx] = h;
    ++pattern_ct;
    /* rehash if load factor too high */
    if (pattern_ct * 4 > (ht_mask + 1) * 3) {     /* >75% */
        ht_expand();
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* ----- Main function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but avoid UB */

    /* Initialise on first call */
    if (!seq_arr) {
        seq_cap = 16;
        seq_arr = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq_arr) abort();

        primes_init();
        ht_init();

        seq_arr[0] = (seq_t)1;           /* a(1) = 1 */
        seq_len = 1;
        last_scanned = 1;
    }

    /* Generate terms up to requested index n */
    while (seq_len < (size_t)n) {
        ++last_scanned;

        uint64_t primes_buf[MAX_P];
        uint8_t   exps_buf[MAX_P];
        size_t    k;

        factorize(last_scanned, primes_buf, exps_buf, &k);

        /* generate all divisors */
        div_cnt = 0;
        gen_divisors(primes_buf, exps_buf, k, 0, 1);

        /* sort them */
        qsort(div_buf, div_cnt, sizeof(Div), cmp_div);

        if (pattern_is_new(exps_buf, k, div_buf, div_cnt)) {
            /* new pattern found – store this number */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_arr = (seq_t*)realloc(seq_arr, seq_cap * sizeof(seq_t));
                if (!seq_arr) abort();
            }
            seq_arr[seq_len++] = (seq_t)last_scanned;
        }
    }

    return seq_arr[n-1];   /* offset 1 */
}
