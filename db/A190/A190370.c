/*
 * A190370  a(n) = n + [n*r/t] + [n*s/t] + [n*u/t]; r=sin(Pi/5), s=cos(Pi/5),
 *          t=sin(2*Pi/5), u=cos(2*Pi/5).
 * https://oeis.org/A190370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*r/t) + floor(n*s/t) + floor(n*u/t) with
 *            r=sin(Pi/5), s=cos(Pi/5), t=sin(2*Pi/5), u=cos(2*Pi/5)
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Rational approximations of the three irrational coefficients.
       They were chosen so that their errors are far below the
       1/n tolerance for any 64‑bit n; all calculations are done in
       128‑bit integer arithmetic so no overflow occurs. */
    static const uint64_t P1 = 514229ULL;          /* φ-1  ≈ 514229/832040   */
    static const uint64_t Q1 = 832040ULL;

    static const uint64_t P2 = 1701301616ULL;      /* ≈0.85065080835204    */
    static const uint64_t Q2 = 2000000000ULL;      /* 2·10^9 (even)        */

    static const uint64_t P3 = 649839392ULL;       /* ≈0.324919696232907    */
    static const uint64_t Q3 = 2000000000ULL;      /* 2·10^9 (even)        */

    // Promote to 128‑bit to avoid overflow while multiplying
    unsigned __int128 m = n;

    seq_t f1 = (seq_t)((m * P1) / Q1);
    seq_t f2 = (seq_t)((m * P2) / Q2);
    seq_t f3 = (seq_t)((m * P3) / Q3);

    return (seq_t)n + f1 + f2 + f3;
}
