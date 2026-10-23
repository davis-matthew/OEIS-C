/*
 * A198080  a(n) = (3^(3*n + 3)- 26*n - 27)/169.
 * https://oeis.org/A198080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(3*n + 3)- 26*n - 27)/169
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t a_val = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t mult = a_val * 27;
        seq_t add  = (seq_t)4 * (seq_t)i + 4;
        a_val = mult + add;
    }
    return a_val;
}
