/*
 * A199028  a(n) = (9*11^n - 1)/2.
 * https://oeis.org/A199028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*11^n - 1)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 11^n using binary exponentiation */
    seq_t exp = (seq_t)1;    /* 11^0 */
    seq_t base = (seq_t)11;  /* base of exponentiation */
    idx_t e = n;             /* exponent in idx_t domain */

    if (e == 0)
        exp = (seq_t)1;
    else
    {
        exp = (seq_t)1;
        while (e > 0)
        {
            if (e & 1)
                exp *= base;
            base *= base;
            e >>= 1;
        }
    }

    /* a(n) = (9*11^n - 1)/2 */
    seq_t temp = exp * (seq_t)9;
    temp = temp - (seq_t)1;
    return temp / (seq_t)2;
}
