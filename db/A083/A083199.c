/*
 * A083199  Exponent of largest power of 2 dividing A061419(n).
 * https://oeis.org/A083199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of largest power of 2 dividing A061419(n), where
 *            A061419(1) = 1, A061419(n) = ceil(3*A061419(n-1)/2)
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 85, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A061419_a(idx_t n)
{
    seq_t v = 1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t half = v >> 1;
        seq_t next = v + half;
        if ((v & 1) != 0) next += 1;
        v = next;
    }
    return v;
}

seq_t a(idx_t n)
{
    seq_t v = A061419_a(n);
    seq_t e = 0;
    if (v == 0) return 0;
    while ((v & 1) == 0) {
        v >>= 1;
        ++e;
    }
    return e;
}
