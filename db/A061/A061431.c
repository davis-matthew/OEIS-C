/*
 * A061431  a(n) = LCM of the n consecutive numbers n(n-1)/2 + 1, ...,
 *          n(n+1)/2.
 * https://oeis.org/A061431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = LCM of the integers n(n-1)/2 + 1, n(n-1)/2 + 2, …,
 *            n(n+1)/2 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t seq_gcd(seq_t a, seq_t b)
{
    /* Euclidean algorithm – works for any unsigned integer type */
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* sequence is defined for n ≥ 1; for safety return 1 for n == 0 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* compute the first term of the block */
    const seq_t start = (seq_t)n * ((seq_t)n - (seq_t)1) / (seq_t)2 + (seq_t)1;

    seq_t res = start;            /* lcm of first element is the element itself */
    for (idx_t i = (idx_t)1; i < n; ++i) {
        const seq_t cur = start + (seq_t)i;
        const seq_t g   = seq_gcd(res, cur);
        res = (res / g) * cur;    /* lcm(res,cur) = res/gcd*cur   */
    }
    return res;
}
