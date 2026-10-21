/*
 * A083885  a(n) = (4^n+2^n+0^n+(-2)^n)/4.
 * https://oeis.org/A083885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4^n + 2^n + 0^n + (-2)^n)/4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast exponentiation for arbitrary‑precision or fixed types */
static seq_t pow_u(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;               /* 0^0 = 1, 4^0 = 2^0 = (-2)^0 = 1 */

    /* compute 4^(n-1) and, if needed, 2^(n-1) */
    seq_t pow4 = pow_u((seq_t)4, n - (idx_t)1);
    if ((n & (idx_t)1) == 0)            /* n even */
    {
        seq_t pow2 = pow_u((seq_t)2, n - (idx_t)1);
        return pow4 + pow2;
    }
    else                                 /* n odd */
    {
        return pow4;
    }
}
