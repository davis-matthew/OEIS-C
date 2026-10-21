/*
 * A059601  Expansion of (1+10*x+5*x^2)/(1-x)^10.
 * https://oeis.org/A059601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+7,7)*(4*n^2+23*n+18)/18
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute binomial(n+7,7) using 128‑bit math */
    unsigned __int128 binom = 1;
    for (idx_t i = 1; i <= 7; ++i) {
        binom = binom * ( (unsigned __int128)(n + i) );
        binom /= i;
    }

    /* polynomial term 4*n^2+23*n+18 */
    unsigned __int128 poly = (unsigned __int128)4 * n * n
                           + (unsigned __int128)23 * n
                           + 18;

    /* final result */
    unsigned __int128 res = binom * poly / 18;
    return (seq_t)res;
}
