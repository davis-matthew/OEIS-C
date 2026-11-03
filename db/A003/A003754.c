/*
 * A003754  Numbers with no adjacent 0's in binary expansion.
 * https://oeis.org/A003754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 4180, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 76 terms through n = 8694 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_no_adjacent_zeros(seq_t k) {
    seq_t bit = k & 1;
    k >>= 1;
    while (k > 0) {
        seq_t next_bit = k & 1;
        if (bit == 0 && next_bit == 0) {
            return false;
        }
        bit = next_bit;
        k >>= 1;
    }
    return true;
}

/* Brute-force enumeration: a(n) is the n-th number with no adjacent 0's in binary */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;
    while (1) {
        if (has_no_adjacent_zeros(k)) {
            count++;
            if (count == n) {
                return k;
            }
        }
        k++;
    }
}
