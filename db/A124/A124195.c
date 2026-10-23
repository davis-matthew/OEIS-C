/*
 * A124195  a(1)=1. a(n) = n - GCD(a(n-1),n).
 * https://oeis.org/A124195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=n - gcd(a(n-1), n) for n>=2
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;       /* a(1) = 1 (and 0/negative indices handled by returning 1) */
    seq_t prev = (seq_t)1;             /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t idx = (seq_t)i;
        seq_t g   = gcd(prev, idx);   /* gcd(a(i-1), i) */
        prev = idx - g;               /* a(i) */
    }
    return prev;                       /* a(n) */
}
