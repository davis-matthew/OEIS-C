/*
 * A071108  CONTINUANT transform of {d(n)}, 1, 2, 2, 3, 2, 4, ... (A000005).
 * https://oeis.org/A071108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 34, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continuit transform of the divisor function:  
   a(0) = 1, a(1) = d(1) = 1, for n ≥ 2
   a(n) = d(n) * a(n-1) + a(n-2), where d(n) = number of positive divisors of n   */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of divisor counts and continuant values   */
    static idx_t      d_max   = 0;            /* largest n up to which d(n) is known   */
    static uint32_t * d_cnt   = NULL;          /* divisor counts, index from 0..d_max    */
    static idx_t      c_max   = 0;            /* largest n up to which a(n) is known   */
    static seq_t  *   c_vals  = NULL;          /* continuant values, index from 0..c_max */

    /* --------------------------------------------------------------------
       Update divisor counts up to n (if needed).  Complexity O(n log n).   */
    if (n > d_max) {
        /* Allocate or grow the divisor array */
        size_t new_size = (size_t)(n + 1);
        d_cnt = (uint32_t *)realloc(d_cnt, new_size * sizeof(uint32_t));
        if (!d_cnt) return 0;                /* out of memory – abort silently   */

        /* Initialise new entries to zero */
        for (idx_t i = d_max + 1; i <= n; ++i)
            d_cnt[i] = 0;

        /* Count divisors by the classic sieve method */
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i)
                ++d_cnt[j];
        }

        d_max = n;
    }

    /* --------------------------------------------------------------------
       Update continuant values up to n (if needed).  Recurrence:
         a(0) = 1, a(1) = d(1), a(n) = d(n)*a(n-1) + a(n-2)   */
    if (n > c_max) {
        /* Allocate or grow the continuant array */
        size_t new_size = (size_t)(n + 1);
        c_vals = (seq_t *)realloc(c_vals, new_size * sizeof(seq_t));
        if (!c_vals) return 0;                /* out of memory – abort silently   */

        /* Initialise base values on first run */
        if (c_max == 0) {
            c_vals[0] = (seq_t)1;            /* a(0) */
            if (n >= 1)
                c_vals[1] = (seq_t)d_cnt[1]; /* a(1) = d(1) = 1 */
            c_max = 1;
        }

        /* Compute new continuant entries */
        for (idx_t i = c_max + 1; i <= n; ++i) {
            c_vals[i] = (seq_t)d_cnt[i] * c_vals[i-1] + c_vals[i-2];
        }

        c_max = n;
    }

    return c_vals[n];
}
