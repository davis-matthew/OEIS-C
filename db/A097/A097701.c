/*
 * A097701  Expansion of 1/((1-x)^2*(1-x^2)^2*(1-x^3)).
 * https://oeis.org/A097701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1) * (9*(-1)^n + n^3 + 17*n^2 + 95*n +
 *            184)/288 + 1/2) = ((n + 1) * (n^3 + 17*n^2 + 95*n + 184 ± 9) +
 *            144) / 288 (± 9: +9 when n is even, –9 when n is odd)
 * Range:     u8 n <= 12, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic so that the intermediate values never
       overflow for typical 64‑bit build sizes.                                      */
    unsigned long long n64 = (unsigned long long)n;   /* convert idx_t to plain integer */
    __int128 n128 = n64;                             /* 128‑bit copy              */
    __int128 np1   = n128 + 1;                       /* (n + 1)                   */

    __int128 base = n128*n128*n128                  /* n^3 */
                 + 17*n128*n128                     /* 17*n^2 */
                 + 95*n128                          /* 95*n   */
                 + 184;                             /* constant term             */

    /* Adjust for the ±9 that depends on the parity of n. */
    if ((n64 & 1ULL) == 0)                          /* n even  --> +9  */
        base += 9;
    else                                            /* n odd   --> –9  */
        base -= 9;

    /* Compute ((n+1)*base + 144) / 288.  The +144 implements the
       +1/2 rounding. */
    __int128 prod = np1 * base + 144;
    __int128 result = prod / 288;

    return (seq_t)result;
}
