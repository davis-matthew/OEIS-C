/*
 * A101813  Odd Niven (or Harshad) numbers: odd numbers that are divisible by
 *          the sum of their digits.
 * https://oeis.org/A101813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1900, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8722 (2026-11-03)
 * Helpers:   digit_sum_idx_to_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th odd Niven (Harshad) number by scanning odd integers
   and testing divisibility by the sum of their digits. */

#include <stdlib.h>
#include "../utils/digit_sum_idx_to_seq.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n<1, but harmless */

    static idx_t candidate = 1;          /* next odd integer to test      */
    static seq_t *cache = NULL;          /* array of found terms           */
    static idx_t cap = 0;                /* current capacity of cache     */
    static idx_t sz  = 0;                /* number of terms stored in cache */

    /* Extend cache until we have at least n terms */
    while (sz < n)
    {
        seq_t sum = digit_sum_idx_to_seq(candidate);

        /* Odd and divisible by the sum of its decimal digits */
        if ((candidate & 1U) && (candidate % (idx_t)sum) == 0)
        {
            if (sz == cap)            /* grow array when needed */
            {
                idx_t new_cap = (cap == 0) ? 16 : (cap << 1);
                seq_t *new_arr = mem_resize(seq_t, cache, new_cap);
                if (!new_arr) return 0; /* allocation failed */
                cache = new_arr;
                cap   = new_cap;
            }
            cache[sz++] = candidate;  /* store the found term */
        }
        candidate += 2;               /* move to next odd number */
    }

    return cache[n - 1];               /* return the requested term */
}
