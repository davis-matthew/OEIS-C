/*
 * A163590  Odd part of the swinging factorial A056040.
 * https://oeis.org/A163590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * n^((-1)^(n+1)) * 2^valuation(n,2) (n>0) with
 *            a(0)=1. This uses the recurrence described in the OEIS entry
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t a_val = 1;
    for (idx_t i = 1; i <= n; ++i) {
        if (i & 1) {                 /* i is odd: multiply by i       */
            a_val *= (seq_t)i;
        } else {                     /* i is even                    */
            /* remove all factors of two: odd_part = i / 2^valuation(i,2) */
            idx_t odd_part = i >> __builtin_ctzll(i);
            a_val /= (seq_t)odd_part;   /* division is exact by definition */
        }
    }
    return a_val;
}
