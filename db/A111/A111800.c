/*
 * A111800  Order of the rote (rooted odd tree with only exponent symmetries)
 *          for n.
 * https://oeis.org/A111800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ ( a(i) + a(e) ) over prime powers p^e || n where i
 *            = index of the prime p (1 for 2, 2 for 3, 3 for 5, …)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ---- utility: integer square root from utils ---------------------------------- */
static inline idx_t integer_sqrt(idx_t x) {
    idx_t r = 0;
    idx_t bit = 1ULL << ((sizeof(idx_t)*8 - 2)&~1ULL);
    while (bit > 0) {
        idx_t nxt = r + bit;
        if (nxt <= x) {
            x -= nxt;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ---- prime table -------------------------------------------------------------- */
static uint64_t *prime_tab = NULL;    /* primes in ascending order          */
static size_t    prime_cnt = 0;       /* number of primes                    */
static uint32_t *prime_index = NULL;  /* prime_index[p] = position (1‑based)*/
static idx_t     prime_max = 0;       /* maximum prime computed              */

static void recompute_primes(idx_t bound) {
    /* Recompute all primes up to 'bound' (inclusive) */
    bool *is_prime = (bool *)malloc((bound + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (idx_t i = 0; i <= bound; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= bound; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= bound; j += i)
                is_prime[j] = false;

    size_t cnt = 0;
    for (idx_t i = 2; i <= bound; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *pr = (uint64_t *)realloc(prime_tab, cnt * sizeof(uint64_t));
    if (!pr) exit(1);
    prime_tab = pr;

    uint32_t *idx = (uint32_t *)realloc(prime_index, (bound + 1) * sizeof(uint32_t));
    if (!idx) exit(1);
    prime_index = idx;
    memset(prime_index, 0, (bound + 1) * sizeof(uint32_t));

    size_t k = 0;
    for (idx_t i = 2; i <= bound; ++i) {
        if (is_prime[i]) {
            prime_tab[k] = i;
            prime_index[i] = (uint32_t)(k + 1); /* 1‑based index */
            ++k;
        }
    }
    prime_cnt = cnt;
    prime_max = bound;
    free(is_prime);
}

static void ensure_primes(idx_t bound) {
    if (bound <= prime_max) return;
    recompute_primes(bound);
}

/* ---- cache for seq values ----------------------------------------------------- */
static seq_t *seq_cache = NULL;      /* 0 means uncomputed, except a(1)=1 */
static size_t  cache_len = 0;        /* allocated size (indexable 0..cache_len) */

/* Ensure cache has room for indices up to 'n' */
static void ensure_cache(idx_t n) {
    if (n < cache_len) return;
    size_t new_len = cache_len ? cache_len * 2 : 64;
    while (new_len <= n) new_len <<= 1;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, (new_len + 1) * sizeof(seq_t));
    if (!new_cache) exit(1);
    for (size_t i = cache_len; i <= new_len; ++i) new_cache[i] = 0;
    seq_cache = new_cache;
    cache_len = new_len;
}

/* Recursive helper that fills cache as needed */
static seq_t compute_a(idx_t n) {
    if (n < cache_len && seq_cache[n] != 0) return seq_cache[n];
    if (n == 1) {       /* base value */
        ensure_cache(1);
        seq_cache[1] = 1;
        return 1;
    }

    /* Ensure we have primes up to n */
    ensure_primes(n);

    idx_t temp = n;
    seq_t sum = 0;

    /* Factorization using primes list */
    for (size_t pi = 0; pi < prime_cnt && prime_tab[pi] <= integer_sqrt(temp); ++pi) {
        uint64_t p = prime_tab[pi];
        if (temp % p != 0) continue;
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        /* compute a(index of prime) + a(exponent) */
        idx_t idxp = prime_index[p];      /* primes are 1‑based */
        ensure_cache(idxp);
        seq_t a_idxp = compute_a(idxp);
        ensure_cache(e);
        seq_t a_e = compute_a(e);
        sum += a_idxp + a_e;
    }

    if (temp > 1) { /* remaining prime factor */
        uint64_t p = temp;
        idx_t e = 1;
        idx_t idxp = prime_index[p];
        ensure_cache(idxp);
        seq_t a_idxp = compute_a(idxp);
        ensure_cache(e);
        seq_t a_e = compute_a(e);
        sum += a_idxp + a_e;
    }

    seq_t result = 1 + sum;
    ensure_cache(n);
    seq_cache[n] = result;
    return result;
}

/* ---- public function ---------------------------------------------------------- */
seq_t a(idx_t n) {
    return compute_a(n);
}
