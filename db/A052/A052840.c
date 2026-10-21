/*
 * A052840  a(n) = n*A029767(n-1).
 * https://oeis.org/A052840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * (2^(n-1) - 1) / (n-1), a(0)=0, a(1)=0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* factorial (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n - 1; ++i)
        fact *= (seq_t)i;

    /* 2^(n-1) */
    seq_t pow2 = 1;
    for (idx_t i = 1; i < n; ++i)
        pow2 <<= 1;

    seq_t result = fact * (pow2 - 1);
    result /= (seq_t)(n - 1);
    result *= (seq_t)n;

    return result;
}
