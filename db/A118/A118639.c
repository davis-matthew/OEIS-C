/*
 * A118639  Smallest number expressible using the next Roman-numeral symbol
 *          under the vinculum system.
 * https://oeis.org/A118639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, fini, full
 * Formula:   a(1) = 1; for n>1, a(n) = 4*10^((n-2)/2) if n even, else a(n) =
 *            9*10^((n-3)/2) if n odd
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Exponentiation by squaring: compute 10^e in seq_t type. */
static seq_t pow10(idx_t e)
{
    seq_t result = (seq_t)1;
    seq_t ten = (seq_t)10;
    while (e)
    {
        if (e & 1ULL)
            result *= ten;
        ten *= ten;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if ((n & 1ULL) == 0) /* even n */
    {
        idx_t exp = (n - 2) / 2;
        return (seq_t)4 * pow10(exp);
    }
    else /* odd n */
    {
        idx_t exp = (n - 3) / 2;
        return (seq_t)9 * pow10(exp);
    }
}
