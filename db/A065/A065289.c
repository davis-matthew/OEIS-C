/*
 * A065289  A065287 conjugated with A059893, inverse of A065290.
 * https://oeis.org/A065289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A059893(A065287(A059893(n)))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A065289_blen(seq_t x)
{
    unsigned L = 0;
    while (x != 0) { x >>= 1; L++; }
    return L;
}

static seq_t A065289_rev(seq_t n)
{
    if (n == 0) return (seq_t)0;
    unsigned L = A065289_blen(n);
    unsigned m = L - 1;
    seq_t prefix = (seq_t)1 << m;
    seq_t r = n - prefix;
    seq_t rev = 0;
    for (unsigned i = 0; i < m; ++i) {
        if ((r >> i) & 1)
            rev |= (seq_t)1 << (m - 1 - i);
    }
    return prefix | rev;
}

static seq_t A065289_perm(seq_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 0) return (seq_t)0;
    unsigned L = A065289_blen(n);
    if (((n >> (L - 2)) & 1) != 0) {
        return (n << 1) | 1;
    }
    if ((n & 1) == 0) {
        return n >> 1;
    }
    seq_t mask = (seq_t)1 << (L - 2);
    return (n | mask) - 1;
}

seq_t a(idx_t n)
{
    seq_t x = A065289_rev((seq_t)n);
    x = A065289_perm(x);
    return A065289_rev(x);
}
