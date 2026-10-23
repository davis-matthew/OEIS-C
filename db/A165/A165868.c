/*
 * A165868  Totally multiplicative sequence with a(p) = 47.
 * https://oeis.org/A165868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 47^bigomega(n) (a totally multiplicative sequence with
 *            a(p)=47)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 63, u64 n <= 4095,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n).  The function is fully self‑contained and uses only basic
   integer arithmetic, so it works for any seq_t and idx_t supplied by
   the build system. */
seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n <= 1)
        return 1;

    idx_t temp = n;
    idx_t count = 0;          /* total number of prime factors (with multiplicity) */

    /* Small trial division factorisation (fast enough for n ≤ ~10⁴) */
    for (idx_t d = 2; d <= temp / d; ++d)
    {
        while (temp % d == 0)
        {
            temp /= d;
            count++;
        }
    }
    if (temp > 1)              /* leftover prime factor */
        count++;

    /* Compute 47^count */
    seq_t result = 1;
    seq_t base   = 47;
    idx_t  exp   = count;

    /* Binary exponentiation */
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    return result;
}
