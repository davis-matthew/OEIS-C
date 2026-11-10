/*
 * A234024  Self-inverse permutation of nonnegative integers,
 *          A059893-conjugate of blue code: a(n) =
 *          A059893(A193231(A059893(n))).
 * https://oeis.org/A234024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A059893(A193231(A059893(n))), where A059893 reverses all
 *            bits of n except the leading 1, and A193231 substitutes x+1 for
 *            x in n read as a GF(2) polynomial
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 84 terms through n = 7747 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A059893_a(seq_t n)
{
    if (n == 0)
        return (seq_t)0;
    unsigned int m = 0;
    seq_t tmp = n;
    while ((tmp >>= 1) != 0)
        ++m;
    seq_t prefix = (seq_t)1 << m;
    seq_t r = n - prefix;
    seq_t rev = 0;
    for (unsigned int i = 0; i < m; ++i) {
        if (((r >> i) & 1) != 0)
            rev |= (seq_t)1 << (m - 1 - i);
    }
    return prefix | rev;
}

static seq_t A193231_a(seq_t n)
{
    seq_t res = 0;
    seq_t mask = 1;
    seq_t t = n;
    while (t != 0) {
        if ((t & 1) != 0)
            res ^= mask;
        mask = (mask << 1) ^ mask;
        t >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return A059893_a(A193231_a(A059893_a(x)));
}
