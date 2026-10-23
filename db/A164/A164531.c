/*
 * A164531  a(1)=1. a(n) = the smallest multiple of d(n) that is >= a(n-1),
 *          where d(n) = the number of positive divisors of n.
 * https://oeis.org/A164531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n >= 2, a(n) = the smallest multiple of d(n) that
 *            is >= a(n-1), where d(n) = number of positive divisors of n
 * Range:     u8 n <= 229, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Local helper: number of divisors of n   (n <= 10000 in practice) */
static inline uint32_t divisor_count(idx_t n) {
    if (n <= 1) return 1;          /* d(1) = 1 */
    uint32_t cnt = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            cnt *= (e + 1);
        }
    }
    if (m > 1) cnt *= 2;           /* last prime factor */
    return cnt;
}

static seq_t *cache = NULL;        /* 1‑based index cache */
static size_t max_computed = 0;    /* highest n that has been computed */
static size_t cache_cap   = 0;     /* current allocation capacity */

/* Ensure the cache can hold up to n (indexed 0…n) */
static void ensure_cache_cap(idx_t n) {
    if ((size_t)n < cache_cap) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* undefined for n < 1 */

    /* Grow cache as needed */
    ensure_cache_cap(n);

    /* Compute missing terms up to n */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t prev = cache[i - 1];
            seq_t d_seq = (seq_t)divisor_count((idx_t)i);
            seq_t t = prev / d_seq;
            if (prev % d_seq) ++t;
            cache[i] = t * d_seq;
        }
    }
    max_computed = (size_t)n;
    return cache[(size_t)n];
}
