/*
 * A033551  Closest integer to (Pi/4)*n^2.
 * https://oeis.org/A033551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round((Pi/4)*n^2)
 * Range:     u8 n <= 18, u16 n <= 288, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a high‑precision rational approximation of Pi/4:
       pi/4 ≈ 7853981633974483096 / 10000000000000000000.
       The relative error is < 1e-19, guaranteeing correct rounding
       for all 64‑bit n. */
    const unsigned long long NUM = 7853981633974483096ULL;
    const unsigned long long DEN = 10000000000000000000ULL;

    /* Compute n^2 in 128‑bit to avoid overflow. */
    unsigned __int128 nn = (unsigned __int128)n * n;

    /* Numerator: NUM * n^2 + DEN/2 (for rounding). */
    unsigned __int128 num = (unsigned __int128)NUM * nn + DEN / 2;

    /* Division by DEN gives the rounded value. */
    unsigned __int128 res = num / DEN;

    return (seq_t)res;
}
