/*
 * A028346  Expansion of 1/((1-x)^4*(1-x^2)^2).
 * https://oeis.org/A028346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+4)*(2*n^4 + 32*n^3 + 172*n^2 + 352*n + 15*(-1)^n +
 *            225)/960
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit integer for intermediate calculation to avoid overflow. */
    __int128 N    = (__int128)n;
    __int128 sign = (n & 1) ? -(__int128)1 : (__int128)1;

    __int128 term = 2*N*N*N*N         /* 2 n^4   */
                  + 32*N*N*N          /* 32 n^3  */
                  + 172*N*N           /* 172 n^2 */
                  + 352*N             /* 352 n   */
                  + 15*sign + 225;    /* ±15 + 225 */

    __int128 num = (N + 4) * term;
    __int128 res = num / 960;         /* division is exact */

    return (seq_t)(unsigned long long)res;
}
