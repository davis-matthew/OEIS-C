/*
 * A071885  Trajectory of 38 under map x -> A002487(x)*A002487(x+1).
 * https://oeis.org/A071885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 38; a(n+1) = A002487(a(n)) * A002487(a(n)+1), where
 *            A002487 is Stern's diatomic series (A002487(2k)=A002487(k),
 *            A002487(2k+1)=A002487(k)+A002487(k+1))
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 18, u64 n <= 24, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 54 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A002487_a(seq_t x) {
    seq_t p = 1, q = 0;
    while (x > 0) {
        if ((x & 1) != 0) q += p;
        else p += q;
        x >>= 1;
    }
    return q;
}

seq_t a(idx_t n) {
    seq_t x = 38;
    for (idx_t i = 0; i < n; i++) {
        x = A002487_a(x) * A002487_a(x + 1);
    }
    return x;
}
