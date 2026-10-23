/*
 * A165530  Number of permutations of length n which avoid the patterns 4321
 *          and 3142.
 * https://oeis.org/A165530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 21*a(n-2) + 20*a(n-3) - 4*a(n-4) for n > 3,
 *            with initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    seq_t t0 = (seq_t)1; /* a(0) */
    seq_t t1 = (seq_t)1; /* a(1) */
    seq_t t2 = (seq_t)2; /* a(2) */
    seq_t t3 = (seq_t)6; /* a(3) */
    seq_t t;

    for (idx_t k = 4; k <= n; ++k) {
        t = (seq_t)8 * t3 - (seq_t)21 * t2 + (seq_t)20 * t1 - (seq_t)4 * t0;
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t;
    }
    return t3;
}
