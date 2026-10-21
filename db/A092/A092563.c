/*
 * A092563  Coefficients in asymptotic expansion of I_0(x)sqrt(2*Pi*x)/e^x in
 *          powers of 1/(16x).
 * https://oeis.org/A092563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(2*n-1)^2 / n * a(n-1), a(0)=1 (equivalent to a(n) =
 *            (2*n)!^2/(n!^3 * 2^n) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    /* Iterate the recurrence a(i) = 2*(2*i-1)^2 / i * a(i-1) */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (sizeof(seq_t) <= 8)   /* 64‑bit or smaller: use 128‑bit temp */
        {
            unsigned __int128 ii = (unsigned __int128)i;
            unsigned __int128 two_m1 = 2 * ii - 1;          /* 2*i-1   */
            unsigned __int128 numer = 2 * two_m1 * two_m1;  /* 2*(2i-1)^2 */
            unsigned __int128 prod   = (unsigned __int128)result * numer;
            result = (seq_t)(prod / ii);
        }
        else                       /* arbitrary‑precision class: let it handle */
        {
            seq_t two      = (seq_t)2;
            seq_t two_i_m1 = (seq_t)(2 * (idx_t)i - 1);
            seq_t numer    = two * two_i_m1 * two_i_m1;
            seq_t prod     = result * numer;
            result         = prod / (seq_t)i;
        }
    }

    return result;
}
