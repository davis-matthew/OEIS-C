/*
 * A053541  a(n) = n*10^(n-1).
 * https://oeis.org/A053541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*10^(n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow10_seq(idx_t e)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)10;
    while (e)
    {
        if (e & 1U)          /* multiply when corresponding bit is set */
            result = result * base;
        e >>= 1;
        if (e)                /* square base for next bit, stop before final shift */
            base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0U)  /* offset is 1; value for n==0 is not used but defined for safety */
        return (seq_t)0;
    seq_t N   = (seq_t)n;
    seq_t pow = pow10_seq(n - 1U);
    return N * pow;
}
