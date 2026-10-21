/*
 * A089173  Solutions x for x^4-1 = kp where p is prime and k=1,2...
 * https://oeis.org/A089173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A089173: list the integers x such that x^4 - 1 = k·p, with p prime,
   ordered by increasing k, then by increasing p. */

/* The implementation precomputes the sequence up to a large enough
   index by generating all prime divisors of x^4-1 for 2 ≤ x ≤ LIMIT
   (LIMIT = 20000), building the pairs (k,p,x), sorting them, and
   extracting the x-values.  The array of values is then returned on
   each subsequent call. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper structure for a pair (k,p,x). */
typedef struct {
    seq_t x;
    seq_t k;
    seq_t p;
} Pair;

/* Static storage for the computed sequence. */
static Pair *pairs      = NULL;
static size_t pair_cap  = 0;
static size_t pair_cnt  = 0;

static seq_t *a_vals    = NULL;
static size_t a_cap     = 0;
static size_t a_cnt     = 0;

static bool initialized = false;

/* Ensure that the pairs array has space for at least 'need' items. */
static void ensure_pair_cap(size_t need)
{
    if (need <= pair_cap) return;
    size_t new_cap = pair_cap ? pair_cap << 1 : 1024;
    while (new_cap < need) new_cap <<= 1;
    pairs = (Pair *)realloc(pairs, new_cap * sizeof(Pair));
    pair_cap = new_cap;
}

/* Ensure that the a_vals array has space for at least 'need' items. */
static void ensure_a_cap(size_t need)
{
    if (need <= a_cap) return;
    size_t new_cap = a_cap ? a_cap << 1 : 1024;
    while (new_cap < need) new_cap <<= 1;
    a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* Comparator for qsort: order by k, then by p. */
static int cmp_pair(const void *va, const void *vb)
{
    const Pair *a = (const Pair *)va;
    const Pair *b = (const Pair *)vb;
    if (a->k < b->k) return -1;
    if (a->k > b->k) return 1;
    if (a->p < b->p) return -1;
    if (a->p > b->p) return 1;
    return 0;
}

/* Prime sieve up to LIMIT. Returns array of primes and their count. */
static void sieve_primes(uint32_t *limit, uint32_t **out, size_t *cnt)
{
    uint32_t LIM = *limit;
    bool *is_prime = (bool *)calloc(LIM + 1, sizeof(bool));
    for (uint32_t i = 2; i <= LIM; ++i) is_prime[i] = true;
    for (uint32_t i = 2; i * i <= LIM; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= LIM; j += i)
                is_prime[j] = false;
        }
    }
    /* Count primes. */
    *cnt = 0;
    for (uint32_t i = 2; i <= LIM; ++i)
        if (is_prime[i]) ++(*cnt);
    *out = (uint32_t *)malloc((*cnt) * sizeof(uint32_t));
    size_t pos = 0;
    for (uint32_t i = 2; i <= LIM; ++i)
        if (is_prime[i]) (*out)[pos++] = i;
    free(is_prime);
}

/* Factor n into distinct prime factors using the supplied primes list. */
static void factor_distinct(uint64_t n, const uint32_t *primes, size_t prime_cnt,
                            uint32_t *out, size_t *out_cnt)
{
    size_t cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            out[cnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) out[cnt++] = (uint32_t)tmp;
    *out_cnt = cnt;
}

/* Initialise the sequence the first time it is requested. */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* Upper bound on x to generate enough terms. */
    const idx_t LIMIT = 20000ull;   /* 20 000 >> 10 000 needed terms */

    /* Sieve primes up to LIMIT for factorisation of x²±1 (needs primes ≤ √(x²+1)). */
    size_t prime_cnt;
    uint32_t *primes;
    sieve_primes((uint32_t *)&LIMIT, &primes, &prime_cnt);

    /* Generate all pairs (k,p,x). */
    for (idx_t x = 2; x <= LIMIT; ++x) {
        uint64_t x2 = (uint64_t)x * (uint64_t)x;
        uint64_t n1 = x2 - 1;        /* x² – 1  */
        uint64_t n2 = x2 + 1;        /* x² + 1  */

        uint32_t f1[16], f2[16];
        size_t c1, c2;

        factor_distinct(n1, primes, prime_cnt, f1, &c1);
        factor_distinct(n2, primes, prime_cnt, f2, &c2);

        /* Merge f1 and f2 into a unique list. */
        uint32_t all[32];
        size_t acnt = 0;
        for (size_t i = 0; i < c1; ++i) all[acnt++] = f1[i];
        for (size_t i = 0; i < c2; ++i) {
            bool present = false;
            for (size_t j = 0; j < acnt; ++j)
                if (all[j] == f2[i]) { present = true; break; }
            if (!present) all[acnt++] = f2[i];
        }

        /* Compute x⁴ as 128‑bit integer to avoid overflow, then cast to seq_t. */
        unsigned __int128 x4_128 = (unsigned __int128)x2 * (unsigned __int128)x2;
        seq_t x4 = (seq_t)x4_128;

        /* Create a pair for each distinct prime divisor. */
        for (size_t i = 0; i < acnt; ++i) {
            seq_t p_seq = (seq_t)all[i];
            seq_t k_val = x4 / p_seq;
            ensure_pair_cap(pair_cnt + 1);
            pairs[pair_cnt].x = (seq_t)x;
            pairs[pair_cnt].k = k_val;
            pairs[pair_cnt].p = p_seq;
            ++pair_cnt;
        }
    }

    free(primes);

    /* Sort pairs by k then p. */
    qsort(pairs, pair_cnt, sizeof(Pair), cmp_pair);

    /* Extract the x-values into the final sequence array. */
    a_cap = pair_cnt;
    a_vals = (seq_t *)malloc(a_cap * sizeof(seq_t));
    for (size_t i = 0; i < pair_cnt; ++i) a_vals[i] = pairs[i].x;

    /* Clean up. */
    free(pairs);
    pairs = NULL;
    pair_cap = 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_sequence();
    /* n is 1‑based; the caller guarantees n ≥ 1. */
    return a_vals[n - 1];
}
