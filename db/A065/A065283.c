/*
 * A065283  A065281 conjugated with A059893, inverse of A065284.
 * https://oeis.org/A065283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A059893(A065281(A059893(n)))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A065283_floorlog2(seq_t v) {
    unsigned m = 0;
    while ((v >>= 1) != 0)
        ++m;
    return m;
}

static seq_t A065283_rev(seq_t v) {
    if (v == 0)
        return (seq_t)0;
    unsigned m = A065283_floorlog2(v);
    seq_t prefix = (seq_t)1 << m;
    seq_t r = v - prefix;
    seq_t rev = 0;
    for (unsigned i = 0; i < m; ++i) {
        if ((r >> i) & 1)
            rev |= (seq_t)1 << (m - 1 - i);
    }
    return prefix | rev;
}

static seq_t A065283_A065281(seq_t x) {
    if (x == 1)
        return (seq_t)1;
    unsigned m = A065283_floorlog2(x);
    seq_t second = (x >> (m - 1)) & 1;
    if (second != 0)
        return x * 2 + 1;
    if ((x & 1) != 0)
        return x >> 1;
    return x | ((seq_t)1 << (m - 1));
}

seq_t a(idx_t n) {
    seq_t x = A065283_rev((seq_t)n);
    x = A065283_A065281(x);
    return A065283_rev(x);
}
