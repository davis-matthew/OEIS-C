/*
 * A027271  a(n) = Sum_{k=0..2n} (k+1)*T(n,k), where T is given by A026536.
 * https://oeis.org/A027271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { (n+1)*6^(n/2) if n even;
 *            4*(floor(n/2)+1)*6^(floor(n/2)) if n odd }
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute 6^exp using exponentiation by squaring */
static seq_t pow6(idx_t exp)
{
    seq_t base   = 6;
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t m = n >> 1;          /* m = floor(n/2) */
    seq_t p = pow6(m);         /* 6^m */

    if ((n & 1) == 0)          /* even n */
    {
        return ((seq_t)(n + 1)) * p;
    }
    else                       /* odd n */
    {
        return ((seq_t)4) * ((seq_t)(m + 1)) * p;
    }
}
