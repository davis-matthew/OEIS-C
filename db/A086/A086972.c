/*
 * A086972  a(n) = n*3^(n-1) + (3^n + 1)/2.
 * https://oeis.org/A086972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*3^(n-1) + (3^n + 1)/2 (offset 0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, used in idx_t definition */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;                /* a(0) = 1 */

    /* Compute 3^n in an iterative manner */
    seq_t pow3 = (seq_t)1;              /* 3^0 = 1 */
    for (idx_t i = 0; i < n; ++i)
        pow3 *= (seq_t)3;               /* multiply by 3 each step */

    /* Compute (3^n + 1)/2  */
    seq_t part1 = (pow3 + (seq_t)1) / (seq_t)2;

    /* Compute n * 3^(n-1)  */
    seq_t pow3_div3 = pow3 / (seq_t)3;  /* 3^(n-1) */
    seq_t part2 = ((seq_t)n) * pow3_div3;

    return part1 + part2;
}
