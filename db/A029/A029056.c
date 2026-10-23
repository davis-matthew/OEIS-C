/*
 * A029056  Expansion of 1/((1-x)*(1-x^3)*(1-x^8)*(1-x^11)).
 * https://oeis.org/A029056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((262*n^3+111*n^2+168*n+432)/288) -
 *            floor((10*n^3+4*n^2+4*n+10)/11)
 * Range:     u8 n <= 62, u16 n <= 458, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic to avoid overflow for the tested range   */
    uint64_t N  = (uint64_t)n;          /* cast index to an integer type */
    uint64_t n2 = N * N;                /* n^2  */
    uint64_t n3 = n2 * N;               /* n^3  */

    uint64_t term1 = 262ULL * n3 + 111ULL * n2 + 168ULL * N + 432ULL;
    uint64_t term2 = 10ULL * n3 + 4ULL * n2 + 4ULL * N + 10ULL;

    uint64_t a1 = term1 / 288ULL;       /* floor((262*n^3+111*n^2+168*n+432)/288) */
    uint64_t a2 = term2 / 11ULL;        /* floor((10*n^3+4*n^2+4*n+10)/11)      */

    uint64_t res = a1 - a2;             /* final result in 64‑bit int */

    return (seq_t)res;                  /* convert to the build’s seq_t type */
}
