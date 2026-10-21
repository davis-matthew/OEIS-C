/*
 * A080220  a(1)=2; then if n is prime, a(n) is the smallest prime number
 *          greater than a(n-1), if n is composite a(n)=a(n-1)+1.
 * https://oeis.org/A080220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2, a(1)=2; if n is prime then a(n) is the smallest prime
 *            greater than a(n-1), otherwise a(n)=a(n-1)+1
 * Range:     u8 n <= 171, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Simple deterministic primality test for 64‑bit values (sufficient for the needed range). */
static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

/* Computes the n‑th term of sequence A080220.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n<1, but caller guarantees n≥1 */

    size_t nn = (size_t)n;
    static seq_t *cache = NULL;
    static size_t cached = 0;

    /* Extend cache if necessary. */
    if (nn > cached) {
        size_t newsize = nn;
        seq_t *newcache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!newcache) {  /* allocation failed */
            exit(EXIT_FAILURE);
        }
        cache = newcache;

        /* Initialize the very first term. */
        if (cached == 0) {
            cache[0] = (seq_t)2;   /* a(1) = 2 */
            cached = 1;
        }

        seq_t cur = cache[cached - 1];
        for (size_t i = cached + 1; i <= nn; ++i) {
            idx_t idx = (idx_t)i;
            if (is_prime_u64((uint64_t)idx)) {
                uint64_t cand = (uint64_t)cur + 1;
                while (!is_prime_u64(cand)) ++cand;
                cur = (seq_t)cand;
            } else {
                cur = cur + 1;
            }
            cache[i - 1] = cur;
        }
        cached = nn;
    }

    return cache[n - 1];
}
