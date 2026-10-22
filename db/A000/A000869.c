/*
 * A000869  Number of switching networks with C(2,n) acting on domain and
 *          GL(3,Z2) acting on range.
 * https://oeis.org/A000869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8^(2^n) + 22*2^(2^n + 2^(n-1) + n) - 22*2^(2^n +
 *            2^(n-1)) + 21*4^(2^n) + 56*2^(2^n) + 42*2^(n + 2^n) +
 *            56*2^(2^(n-1) + n) - 56*2^(2^(n-1)) + 48*2^n)/(168*2^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a869_from_u128, a869_is_fixed, a869_p2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a869_from_u128.h"
#include "../utils/a869_is_fixed.h"
#include "../utils/a869_p2.h"

seq_t a(idx_t n) {
    idx_t N = (idx_t)1 << n;        /* 2^n */
    idx_t M = (idx_t)1 << (n - 1);  /* 2^(n-1) */
    /* numerator / 2^n split into positive and negative parts */
    if (a869_is_fixed()) {
        unsigned __int128 one = 1, pos, neg;
        if (n > 5) return 0; /* value exceeds 128 bits */
        pos = (one << (3 * N - n)) + 22 * (one << (N + M)) + 21 * (one << (2 * N - n))
            + 56 * (one << (N - n)) + 42 * (one << N) + 56 * (one << M) + 48;
        neg = 22 * (one << (N + M - n)) + 56 * (one << (M - n));
        return a869_from_u128((pos - neg) / 168);
    } else {
        seq_t pos = a869_p2(3 * N - n);
        seq_t neg;
        pos += a869_p2(N + M) * 22;
        pos += a869_p2(2 * N - n) * 21;
        pos += a869_p2(N - n) * 56;
        pos += a869_p2(N) * 42;
        pos += a869_p2(M) * 56;
        pos += 48;
        neg = a869_p2(N + M - n) * 22;
        neg += a869_p2(M - n) * 56;
        return (pos - neg) / 168;
    }
}
