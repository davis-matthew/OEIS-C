/*
 * A031441  a(0) = 3; for n > 0, a(n) is the greatest prime factor of
 *          PreviousPrime(a(n-1))*a(n-1)-1 where
 *          PreviousPrime(prime(k))=prime(k-1).
 * https://oeis.org/A031441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gpf, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 3;
 * a(n) = gpf(prev_prime(a(n-1)) * a(n-1) - 1)   for n > 0
 * where gpf(x) is the greatest prime factor of x
 * and prev_prime(p) returns the largest prime < p
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gpf.h"        /* uint64_t gpf(uint64_t x); */
#include "../utils/prev_prime.h" /* idx_t prev_prime(idx_t n); */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of terms */
    static size_t cache_len = 0;    /* allocated size */
    static size_t cur_len   = 0;    /* number of terms computed */

    /* grow array if needed */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)                 /* allocation failure → zero */
            return (seq_t)0;
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing values up to n */
    for (size_t i = cur_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)3;
        } else {
            seq_t prev = cache[i - 1];
            idx_t smaller_prime = prev_prime((idx_t)prev);
            uint64_t prod = ((uint64_t)smaller_prime) * ((uint64_t)prev) - 1;
            cache[i] = (seq_t)gpf(prod);
        }
    }
    cur_len = (size_t)n + 1;
    return cache[(size_t)n];
}
