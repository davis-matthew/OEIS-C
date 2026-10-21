/*
 * A202237  Odd numbers with the same number of prime factors of the form
 *          4*k+1 and 4*k+3.
 * https://oeis.org/A202237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th odd number with an equal number of prime factors
 *            of the form 4*k+1 and 4*k+3 (prime factors counted with
 *            multiplicity). The sequence starts 1, 15, 35, ... This
 *            implementation generates terms on demand by scanning odd
 *            numbers, factoring each by trial division, and caching the
 *            results
 * Range:     u8 n <= 24, u16 n <= 7144, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache: first element is always 1 */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static seq_t next_candidate = 3;  /* next odd number to test */

    if (n == 0) return 0;  /* undefined for n == 0, return 0 as placeholder */

    /* initialize cache on first call */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;  /* allocation failure */
        cache[0] = 1;          /* first term is 1 */
        cache_len = 1;
    }

    /* extend cache until we have n terms */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;  /* candidate odd number */
        uint64_t m = (uint64_t)cand;  /* work with 64‑bit value for factorisation */
        int c1 = 0, c3 = 0;           /* counts of primes ≡1 and ≡3 (mod 4) */

        /* factorise m by trial division */
        while (m > 1) {
            uint64_t p = 0;

            /* find the smallest odd divisor */
            for (uint64_t d = 3; d <= m / d; d += 2) {
                if (m % d == 0) {
                    p = d;
                    break;
                }
            }
            if (!p) p = m;  /* m is prime itself */

            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }

            if (p % 4 == 1) c1 += exp;
            else if (p % 4 == 3) c3 += exp;
        }

        if (c1 == c3) {
            /* store candidate if it satisfies the property */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
                if (!tmp) return 0;  /* reallocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        next_candidate += 2;  /* only odd numbers needed */
    }

    return cache[n - 1];
}
