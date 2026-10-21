/*
 * A229572  Number of defective 4-colorings of an n X 2 0..3 array connected
 *          horizontally, vertically, diagonally and antidiagonally with
 *          exactly one mistake, and colors introduced in row-major 0..3
 *          order.
 * https://oeis.org/A229572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n==1, else 2^(n-2) * (7*n-8) (n>1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Multiply by 2, 'exp' times */
static seq_t power_of_two(idx_t exp)
{
    seq_t r = 1;
    while (exp) {
        r *= 2;
        --exp;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, guard */
    if (n == 1) return 1;          /* a(1) = 1 */
    seq_t pow2 = power_of_two(n - 2);
    seq_t mult = 7 * (seq_t)n - 8; /* (7*n - 8) */
    return pow2 * mult;
}
