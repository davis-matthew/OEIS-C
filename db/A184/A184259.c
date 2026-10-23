/*
 * A184259  Number of strings of numbers x(i=1..4) in 0..n with sum i^3*x(i)
 *          equal to 64*n.
 * https://oeis.org/A184259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 160, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Number of solutions to the equation
 *    x1 + 8*x2 + 27*x3 + 64*x4 = 64*n
 * with 0 <= xi <= n (for i=1..4).  This is OEIS A184259.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1, but handle zero safely */
    seq_t lower = (seq_t)63 * n;  /* 63n  = 64n - n */
    seq_t upper = (seq_t)64 * n;  /* 64n  */

    seq_t count = 0;

    for (idx_t x4 = 0; x4 <= n; ++x4)
    {
        seq_t term64x4 = (seq_t)64 * x4;
        for (idx_t x3 = 0; x3 <= n; ++x3)
        {
            seq_t base = term64x4 + (seq_t)27 * x3;  /* 64*x4 + 27*x3 */

            /* Compute lower and upper differences using signed arithmetic
               to avoid unsigned wraparound when base > lower. */
            int64_t diff_min = (int64_t)lower - (int64_t)base;   /* 63n - base */
            int64_t diff_max = (int64_t)upper - (int64_t)base;   /* 64n - base */

            if (diff_max < 0) continue;          /* S > 64n – impossible */

            idx_t x2_low;
            if (diff_min <= 0)
                x2_low = 0;                     /* base already >= 63n – no need for ceil */
            else
                x2_low = (idx_t)((diff_min + 7) / 8);  /* ceil((63n - base)/8) */

            idx_t x2_high = (idx_t)(diff_max / 8);      /* floor((64n - base)/8) */

            if (x2_low > n) continue;           /* lower bound beyond allowed */

            if (x2_high > n) x2_high = n;        /* clamp to xi <= n */

            if (x2_low <= x2_high)
            {
                count += (seq_t)(x2_high - x2_low + 1);
            }
        }
    }

    return count;
}
