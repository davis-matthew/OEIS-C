/*
 * A004084  a(n) = n-th positive integer k such that tan(k-1) <= 0 and tan(k)
 *          > 0.
 * https://oeis.org/A004084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(Pi*(n-1)) for n>1, a(1) = 1; computed exactly
 *            with 64-bit fixed-point Pi (Pi*m is never an integer, so ceiling
 *            = floor + 1)
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;
    uint64_t m = (uint64_t)(n - 1);
    /* fractional part of Pi times 2^64: Pi = 3.243F6A8885A308D3 13198A2E... (hex) */
    const uint64_t f = 0x243F6A8885A308D3ULL;
    unsigned __int128 prod = (unsigned __int128)m * f;
    uint64_t fl = (uint64_t)(prod >> 64); /* floor(m * frac(Pi)) */
    seq_t r = (seq_t)m;
    r = r * 3;
    r += (seq_t)fl;
    r += 1;
    return r;
}
