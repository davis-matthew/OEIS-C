/*
 * A028423  Clog sequence in base 2. Right to left concatenation of
 *          n,int(log_2(n)),int(log_2(int(log_2(n)))),... in base 2.
 * https://oeis.org/A028423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of binary representations of n,
 *            floor(log2(n)), floor(log2(floor(log2(n)))), ... stopping before
 *            0
 * Range:     u8 n <= 15, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   bitlen_seq_t, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n) {
    /* n is guaranteed >= 1 per OEIS offset. */
    seq_t result = (seq_t)n;
    idx_t current = floor_log2_uint64(n);
    while (current > 0) {
        unsigned bits = bitlen_seq_t(result);
        result = ((seq_t)current << bits) | result;
        current = floor_log2_uint64(current);
    }
    return result;
}
