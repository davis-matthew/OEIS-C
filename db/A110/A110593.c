/*
 * A110593  a(1) = 3, a(n+1) = 2*(3^n).
 * https://oeis.org/A110593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, less, nonn
 * Formula:   a(1) = 3, a(n) = 2 * 3^(n-1) for n > 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 3^exp using binary exponentiation. */
static seq_t pow3(idx_t exp)
{
    seq_t base = (seq_t)3;
    seq_t res  = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;
    seq_t val = pow3(n - 1);
    val *= (seq_t)2;
    return val;
}
