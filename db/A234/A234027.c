/*
 * A234027  Self-inverse permutation of nonnegative integers,
 *          A054429-conjugate of blue code: a(n) =
 *          A054429(A193231(A054429(n))).
 * https://oeis.org/A234027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A054429(A193231(A054429(n))), with a(0) = 0
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 84 terms through n = 7604 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A054429_a(seq_t x)
{
    if (x == 0) return (seq_t)0;
    seq_t start = 1;
    seq_t t = x;
    while (t > 1) {
        t >>= 1;
        start <<= 1;
    }
    /* reverse the block [start, 2*start-1] */
    return start + start + start - 1 - x;
}

static seq_t A193231_a(seq_t x)
{
    seq_t res = 0;
    seq_t mask = 1;
    seq_t t = x;
    while (t) {
        if (t & 1)
            res ^= mask;
        mask = (mask << 1) ^ mask;
        t >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t x = A054429_a((seq_t)n);
    x = A193231_a(x);
    return A054429_a(x);
}
