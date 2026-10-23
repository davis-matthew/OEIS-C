/*
 * A024203  [ (4th elementary symmetric function of S(n))/(first elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+3 odd
 *          positive integers}.
 * https://oeis.org/A024203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A024203: a(n) = floor(A024198(n)/(n+3)^2), where
 * A024198(n) is the 4th elementary symmetric sum of the first
 * (n+3) odd positive integers.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 1 – return 0 for n < 1. */
    if (n < 1)
        return 0;

    /* m = n+3 is the number of odd integers in S(n). */
    uint64_t m = (uint64_t)n + 3;
    /* using 128‑bit intermediate arithmetic to avoid overflow
     * even when seq_t is 64 bits.  __int128 is supported by
     * the compilers used for the OEIS tests. */
    __int128 m128 = (__int128)m;

    /* power sums of the first m odd numbers:
     * S1 = Σ(2k-1)   = m^2
     * S2 = Σ(2k-1)^2 = 4Σk^2 - 4Σk + m
     * S3 = Σ(2k-1)^3 = 8Σk^3 - 12Σk^2 + 6Σk - m
     * S4 = Σ(2k-1)^4 = 16Σk^4 - 32Σk^3 + 24Σk^2 - 8Σk + m
     */
    __int128 sumk   = m128 * (m128 + 1) / 2;                       /* Σk          */
    __int128 sumk2  = m128 * (m128 + 1) * (2 * m128 + 1) / 6;     /* Σk^2        */
    __int128 sumk3  = sumk * sumk;                                 /* Σk^3        */
    __int128 sumk4  = m128 * (m128 + 1) * (2 * m128 + 1) *
                      (3 * m128 * m128 + 3 * m128 - 1) / 30;      /* Σk^4        */

    __int128 S1 = m128 * m128;                                     /* Σ(2k-1)     */
    __int128 S2 = 4 * sumk2 - 4 * sumk + m128;                     /* Σ(2k-1)^2   */
    __int128 S3 = 8 * sumk3 - 12 * sumk2 + 6 * sumk - m128;        /* Σ(2k-1)^3   */
    __int128 S4 = 16 * sumk4 - 32 * sumk3 + 24 * sumk2 - 8 * sumk + m128; /* Σ(2k-1)^4 */

    /* Newton identity for e4 (the 4th elementary symmetric sum):
     * e4 = (S1^4 - 6 S1^2 S2 + 3 S2^2 + 8 S1 S3 - 6 S4) / 24   */
    __int128 numerator = S1 * S1 * S1 * S1
                       - 6 * S1 * S1 * S2
                       + 3 * S2 * S2
                       + 8 * S1 * S3
                       - 6 * S4;
    __int128 e4 = numerator / 24;          /* exact integer */

    /* final term a(n) = floor(e4 / m^2) */
    __int128 denom = m128 * m128;
    __int128 res128 = e4 / denom;

    /* cast the result back to seq_t; for seq_t types smaller
     * than 128 bits the value may wrap if it would overflow,
     * but such values are beyond the range where the sequence
     * is required to be exact. */
    return (seq_t)res128;
}
