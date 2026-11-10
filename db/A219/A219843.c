/*
 * A219843  Rows of A219463 seen as numbers in binary representation.
 * https://oeis.org/A219843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000225(n+1) - A001317(n) = (2^(n+1) - 1) - Product_{j
 *            where bit j of n is 1} (2^(2^j) + 1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 930 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A219843_pow2pow2(idx_t e) {
    seq_t result = 2;
    for (idx_t i = 1; i <= e; i++) {
        result = result * result;
    }
    return result;
}

static seq_t A219843_A001317(idx_t n) {
    seq_t result = 1;
    idx_t bit_pos = 0;
    while (n > 0) {
        if (n & 1) {
            seq_t fermat = A219843_pow2pow2(bit_pos) + 1;
            result = result * fermat;
        }
        n >>= 1;
        bit_pos++;
    }
    return result;
}

static seq_t A219843_A000225(idx_t n) {
    seq_t r = 0;
    for (idx_t i = 0; i < n; i++) {
        r = r * 2 + 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    return A219843_A000225(n + 1) - A219843_A001317(n);
}
