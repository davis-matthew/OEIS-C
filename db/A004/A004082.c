/*
 * A004082  Numbers k such that sin(k-1) <= 0 and sin(k) > 0.
 * https://oeis.org/A004082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = ceiling(2*Pi*(n-1)) for n >= 2 (the k with
 *            sin(k-1) <= 0 < sin(k) are k = ceiling(2*Pi*j))
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    uint64_t m = (uint64_t)(n - 1);
    /* floor(2*Pi * 10^18) */
    unsigned __int128 pi2_scaled = (unsigned __int128)6283185307179586476ULL;
    unsigned __int128 scale = (unsigned __int128)1000000000000000000ULL;
    unsigned __int128 numerator = pi2_scaled * (unsigned __int128)m;
    unsigned __int128 quotient = numerator / scale;
    unsigned __int128 remainder = numerator % scale;

    /* 2*Pi*m is never an integer, so the ceiling is floor + 1 */
    if (remainder > 0) quotient++;

    uint64_t q = (uint64_t)quotient;
    return (seq_t)q;
}
