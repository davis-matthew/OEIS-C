/*
 * A167424  Define a sequence of fractions by f(1) = 1/2, f(n+1) = (f(n)^2 +
 *          1)/2; sequence gives numerators.
 * https://oeis.org/A167424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(0) = 0; a(n+1) = a(n)^2 + 4^(2^n-1) (n ≥ 0)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a_val holds a(k), d_val holds 4^(2^k-1) for the current k   */
    seq_t a_val = 0;
    seq_t d_val = 1;   /* d_0 = 4^(2^0-1) = 1 */

    for (idx_t k = 0; k < n; ++k) {
        /* compute next terms iteratively */
        seq_t next_a = a_val * a_val + d_val;                 /* a(k+1) = a(k)^2 + d(k)   */
        seq_t next_d = (d_val * d_val) * (seq_t)4;           /* d(k+1) = 4 * d(k)^2       */

        a_val = next_a;
        d_val = next_d;
    }
    return a_val;
}
