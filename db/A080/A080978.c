/*
 * A080978  a(n) = 2*A006046(n) + 1.
 * https://oeis.org/A080978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A006046(n) + 1 A006046(0) = 0 If n is a power of two,
 *            A006046(n) = 3^(log2 n) Otherwise let m = 2^⌊log2 n⌋ (the
 *            largest power of two ≤ n) Then A006046(n) = A006046(m) +
 *            2*A006046(n - m) Each intermediate value is calculated with type
 *            seq_t, using only integer arithmetic allowed by the SE...
 * Range:     u8 n <= 23, u16 n <= 767, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 3^k as a seq_t.  */
static seq_t pow3(int k)
{
    seq_t r = 1;
    for (int i = 0; i < k; ++i)
        r *= (seq_t)3;
    return r;
}

/* Recursive helper for A006046.  */
static seq_t a006046_rec(idx_t n)
{
    if (n == 0)
        return 0;

    /* largest power of two <= n */
    idx_t p = 1;
    while (p <= n / 2)
        p <<= 1;

    if (n == p) {
        /* p = 2^k, compute k */
        int k = 0;
        idx_t t = p;
        while (t > 1) {
            t >>= 1;
            ++k;
        }
        return pow3(k);
    } else {
        return a006046_rec(p) + (seq_t)2 * a006046_rec(n - p);
    }
}

/* Public entry point.  */
seq_t a(idx_t n)
{
    seq_t v = a006046_rec(n);
    return (seq_t)2 * v + (seq_t)1;
}
