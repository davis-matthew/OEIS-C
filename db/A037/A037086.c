/*
 * A037086  Beatty sequence for sqrt(Pi).
 * https://oeis.org/A037086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(pi))
 * Range:     u8 n <= 144, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑computed integer approximation:
       sqrt(pi) ≈ 1.7724538509055160272981675...
       Multiply by 2^20 = 1048576 and round to the nearest integer. */
    static const unsigned int SCALE_BITS   = 20U;
    static const unsigned long long  FACTOR = 1858553ULL; /* sqrt(pi) * 2^20 rounded */

    unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)FACTOR;
    prod >>= SCALE_BITS;                 /* divide by 2^20 */
    return (seq_t)prod;
}
