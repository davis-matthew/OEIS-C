/*
 * A208705  Number of n X 4 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 4*a(n-2) with a(1)=8, a(2)=100
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply a seq_t value by a small unsigned integer without overflow. */
static inline seq_t mul_const(seq_t x, unsigned int c)
{
    /* For builtin integer sizes we use a 128‑bit intermediate. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 tmp = (unsigned __int128)x * (unsigned __int128)c;
        return (seq_t)tmp;
    }
    /* For larger (e.g., arbitrary‑precision) types the multiplication
       is handled by the type’s own operator*. */
    return x * (seq_t)c;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)100;

    seq_t prev2 = (seq_t)8;
    seq_t prev1 = (seq_t)100;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = mul_const(prev1, 13) - mul_const(prev2, 4);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
