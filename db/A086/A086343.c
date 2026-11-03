/*
 * A086343  a(n) starts new run of consecutive values in A055938.
 * https://oeis.org/A086343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4n - 2 - popcount(n-1)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   popcount_idx_to_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/popcount_idx_to_i32.h"

seq_t a(idx_t n) {
    /* Guard against a zero index; the sequence is defined for n >= 1. */
    if (n == 0)
        return (seq_t)0;

    seq_t n_seq = (seq_t)n;
    seq_t val    = (seq_t)4 * n_seq - (seq_t)2;
    int pc       = popcount_idx_to_i32(n - 1u);
    val -= (seq_t)pc;
    return val;
}
