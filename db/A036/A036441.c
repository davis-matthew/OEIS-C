/*
 * A036441  a(n+1) = next number having largest prime dividing a(n) as a
 *          factor, with a(1) = 2.
 * https://oeis.org/A036441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nice, nonn
 * Formula:   a(1) = 2; a(n+1) = next integer > a(n) divisible * by the
 *            largest prime factor of a(n). Equivalently, * a(n+1) =
 *            ((a(n)/p)+1)*p where p = largest_prime_divisor(a(n))
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/largest_prime_divisor.h"

/* Compute a(n).  The function memoises values so
 * that any order of calls up to a certain maximum works
 * in O(n) time for each new index.  The recursion is
 * trivial, but memoisation guarantees the efficiency
 * requirement for arbitrary call order. */
seq_t a(idx_t n)
{
    /* return 0 for non‑positive index (undefined behaviour otherwise) */
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;   /* memoised sequence values */
    static idx_t max_cached = 0;   /* highest n that is cached */

    /* Allocate or grow cache to hold up to n entries */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * n);
        if (cache == NULL) exit(1);
    } else if (n > max_cached) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (tmp == NULL) exit(1);
        cache = tmp;
    }

    /* Compute missing terms if necessary */
    if (n > max_cached) {
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i - 1] = (seq_t)2;
            } else {
                seq_t prev = cache[i - 2];                    /* a(i-1) */
                uint64_t p = largest_prime_divisor((uint64_t)prev);
                seq_t q   = prev / (seq_t)p;                 /* floor(prev/p) */
                seq_t next = (q + 1) * (seq_t)p;             /* next multiple of p > prev */
                cache[i - 1] = next;
            }
        }
        max_cached = n;
    }

    return cache[n - 1];
}
