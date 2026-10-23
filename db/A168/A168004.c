/*
 * A168004  Numbers k with property that first digit of 8*k = last digit of k.
 * https://oeis.org/A168004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 881, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n-th positive integer k such that the first digit of 8*k
   equals the last digit of k.  Offsets: a(1) = 21, a(2) = 32, ...  */

/* OEIS A168004 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached sequence values */
    static size_t  sz    = 0;       /* current number of cached values   */
    static size_t  cap   = 0;       /* current capacity of cache         */
    static idx_t   last_k = 0;      /* last k examined for the property */

    /* ensure at least one slot if first call */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend cache until we have n terms */
    while (sz < n) {
        /* search for next k that satisfies the property */
        seq_t kk;
        seq_t last_digit;
        seq_t prod;
        seq_t first_digit;

        /* next candidate k */
        do {
            ++last_k;
            kk = (seq_t)last_k;
            last_digit = kk % 10;       /* last digit of k   */
        } while (last_digit == 0);       /* t cannot be 0 */

        /* compute 8*k */
        prod = kk * 8;

        /* find first digit of prod */
        first_digit = prod;
        while (first_digit >= 10)
            first_digit /= 10;

        if (first_digit == last_digit) {
            /* store kk in cache */
            if (sz >= cap) {
                cap   *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[sz++] = kk;
        }
    }

    return cache[n - 1];
}
