/*
 * A088236  Total number of digits (in base 2) in all preceding terms in the
 *          sequence.
 * https://oeis.org/A088236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n+1) = a(n) + floor(log_2 a(n)) + 1, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 40, u16 n <= 4424, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 73 terms through n = 4857 (2026-11-01)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bit_length.h"

/*  OEIS A088236  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t val = (seq_t)1;          /* a(1) */
    if (n == 1) return val;

    for (idx_t i = 2; i <= n; ++i) {
        /* floor(log2(val)) + 1 == bitlen_seq_t(val) */
        val += bit_length(val);
    }
    return val;
}
