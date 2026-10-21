/*
 * A088161  n rotated one binary place to the right less n rotated one binary
 *          place to the left.
 * https://oeis.org/A088161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = A038572(n) – A006257(n) That is, a(n) = (n rotated one
 *            binary place to the right) minus (n rotated one binary place to
 *            the left)
 * Range:     i8 n <= 128, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n = 0 has 0 bits; the difference is defined to be 0. */
    if (n == 0)
        return 0;

    /* Determine the bit‑length k of n (k ≥ 1).  */
    idx_t k = 0, tmp = n;
    while (tmp) {
        ++k;
        tmp >>= 1;
    }

    /* k-bit mask: 0…0 111…1 (k ones).  */
    uint64_t mask = ~0ULL >> (64 - k);

    /* Work in unsigned 64‑bit arithmetic.  */
    uint64_t N = (uint64_t)n;

    /* Rotated right: shift right 1, bring LSB to MSB.  */
    uint64_t rrot = ((N >> 1) | ((N & 1ULL) << (k - 1))) & mask;

    /* Rotated left: shift left 1, bring MSB to LSB.  */
    uint64_t lrot = ((N << 1) | (N >> (k - 1))) & mask;

    /* Convert to seq_t and compute the signed difference.  */
    seq_t right  = (seq_t)rrot;
    seq_t left   = (seq_t)lrot;
    return right - left;
}
