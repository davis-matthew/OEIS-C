/*
 * A024195  Integer part of (4th elementary symmetric function of S(n))/(3rd
 *          elementary symmetric of S(n)), where S(n) = {3,4, ..., n+5}.
 * https://oeis.org/A024195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( e4 / e3 ) where e3 and e4 are the 3rd and 4th
 *            elementary symmetric sums of the set S(n) = {3, 4, …, n+5}. The
 *            sums are computed via the Newton identities using power sums up
 *            to fourth power. All intermediate computations are performed in
 *            128‑bit integers to avoid overflow for n up t...
 * Range:     u8 n <= 41, u16 n <= 720, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1, but guard 0 */

    /* m = |S(n)| = n + 3 */
    __int128 M = (__int128)n + 3;
    const __int128 x = 3;                    /* first element of S(n) */

    /* Sums of powers of k = 0 .. M-1 */
    __int128 sum_k   = M * (M - 1) / 2;
    __int128 sum_k2  = M * (M - 1) * (2 * M - 1) / 6;
    __int128 sum_k3  = sum_k * sum_k;
    __int128 sum_k4  = M * (M - 1) * (2 * M - 1)
                       * (3 * M * M - 3 * M - 1) / 30;

    /* Power sums of the sequence a_i = x + k */
    __int128 p1 = x * M + sum_k;                                   // Σ a_i
    __int128 p2 = x * x * M + 2 * x * sum_k + sum_k2;             // Σ a_i²
    __int128 p3 = x * x * x * M + 3 * x * x * sum_k
                  + 3 * x * sum_k2 + sum_k3;                       // Σ a_i³
    __int128 p4 = x * x * x * x * M + 4 * x * x * x * sum_k
                  + 6 * x * x * sum_k2 + 4 * x * sum_k3 + sum_k4; // Σ a_i⁴

    /* Elementary symmetric sums via Newton identities */
    __int128 e1 = p1;
    __int128 e2 = (p1 * p1 - p2) / 2;
    __int128 e3 = (p1 * e2 - e1 * p2 + p3) / 3;
    __int128 e4 = (p1 * e3 - e2 * p2 + e1 * p3 - p4) / 4;

    /* Floor of e4 / e3 */
    return (seq_t)(e4 / e3);
}
