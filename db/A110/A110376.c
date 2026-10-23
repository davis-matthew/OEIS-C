/*
 * A110376  a(n) = Sum_{r < n, gcd(r,n)=1} n!/r.
 * https://oeis.org/A110376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{r < n, gcd(r,n)=1} n!/r
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for idx_t (uint64_t) */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;           /* a(1) = 1, a(0) unused but safe */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)         /* compute n! */
        fact *= (seq_t)i;

    seq_t sum = (seq_t)0;
    for (idx_t r = 1; r < n; ++r)          /* sum over coprime r */
        if (gcd_idx(r, n) == 1)
            sum += fact / (seq_t)r;
    return sum;
}
