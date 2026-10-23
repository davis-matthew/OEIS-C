/*
 * A029194  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^6)*(1-x^8)).
 * https://oeis.org/A029194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + (63+15*(-1)^n)*n^2 + (597+315*(-1)^n)*n +
 *            4330 + 1430*(-1)^n)/5760)
 * Range:     u8 n <= 77, u16 n <= 561, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use __int128 for intermediate calculations to avoid overflow */
    __int128 nn = (unsigned long long)n;      /* cast to unsigned to guarantee a non‑negative value */
    int sgn = (n & 1ULL) ? -1 : 1;            /* (-1)^n */

    /* Compute the numerator in signed 128‑bit */
    __int128 num = 2*nn*nn*nn
                 + (__int128)(63 + 15*sgn) * nn*nn
                 + (__int128)(597 + 315*sgn) * nn
                 + 4330
                 + 1430*sgn;

    /* Division yields the exact floor value (non‑negative). */
    return (seq_t)(num / 5760);
}
