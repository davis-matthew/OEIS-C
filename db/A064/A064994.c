/*
 * A064994  A Beatty sequence from Khinchin's constant (A002210).
 * https://oeis.org/A064994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n * (K - 1) ), where K is Khinchin's constant (K ≈
 *            2.685452001260189). Here (K - 1) ≈ 1.685452001260189. The value
 *            is computed as floor(n * NUM / DEN) with a 15‑digit rational
 *            approximation of (K‑1) that is accurate enough for all n up to
 *            10 000 (and in practice for any n that yield...
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t NUM = 1685452001260189ULL;  /* (K-1) * 10^15, rounded down */
static const uint64_t DEN = 1000000000000000ULL;  /* 10^15 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the multiplication to avoid overflow
       before the division.  The product n * NUM fits comfortably in
       unsigned __int128 for all reasonable n (n <= 1e9 gives <2^128).   */
    __uint128_t prod = (__uint128_t)n * NUM;
    return (seq_t)(prod / DEN);
}
