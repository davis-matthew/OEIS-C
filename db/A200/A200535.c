/*
 * A200535  G.f. satisfies: A(x) = exp( Sum_{n>=1} [Sum_{k=0..2*n} C(2*n,k)^2
 *          * x^k] / A(x)^n * x^n/n ).
 * https://oeis.org/A200535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=4, a(3)=5, a(4)=9; for n>=5, a(n)=4*n-8
 * Range:     u8 n <= 65, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* small n handled by lookup table, larger n by closed form */
    static const seq_t small[5] = {1, 1, 4, 5, 9};

    if (n < 5)
        return small[(size_t)n];

    /* a(n) = 4*n - 8, computed in seq_t to keep operations small */
    seq_t s_n = (seq_t)n;
    seq_t res = (seq_t)4 * s_n - (seq_t)8;
    return res;
}
