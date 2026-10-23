/*
 * A181448  Numbers k such that 5 is the largest prime factor of k^2 - 1.
 * https://oeis.org/A181448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = the n-th integer k such that the largest prime factor of
 *            k^2-1 is 5
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is finite: it contains exactly the terms {4,9,11,19,26,31,49,161} */
/* The algorithm tests successive k, checks that 5 divides k^2-1 and that every prime factor of k^2-1 is ≤5. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;
    static uint64_t next_k = 2;   /* next k to test */

    /* Resize helper. */
    if (sz >= cap) {
        size_t new_cap = cap == 0 ? 8 : cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed, return 0 as error */
        cache = tmp;
        cap   = new_cap;
    }

    /* Ensure cache has at least n entries. */
    while (sz < n) {
        uint64_t k = next_k++;
        /* Compute k^2 - 1.  Use seq_t to avoid overflow on large builds. */
        seq_t m = (seq_t)k * (seq_t)k - 1;
        /* 5 must divide m. */
        if (m % (seq_t)5 == 0) {
            seq_t temp = m;
            /* Remove all factors 2,3,5. */
            while (temp % (seq_t)2 == 0) temp /= 2;
            while (temp % (seq_t)3 == 0) temp /= 3;
            while (temp % (seq_t)5 == 0) temp /= 5;
            if (temp == 0) {
                /* Never happens, but keep for safety. */
                temp = 1;
            }
            if (temp == 1) {
                /* Now the largest prime factor is 5. */
                cache[sz++] = (seq_t)k;
                continue;
            }
        }
        /* Else skip this k. */
    }

    return cache[n-1];
}
