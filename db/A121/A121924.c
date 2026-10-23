/*
 * A121924  Number of splitting steps that one can take with a sequence of n
 *          2's.
 * https://oeis.org/A121924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 2379, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A121924: Number of splitting steps that one can take with a sequence of n 2's.
   Formula (per OEIS entry): a(n) = binomial(b,3) + (n - binomial(b,2))*(b*b+3b-2(n+1))/4,
   where b = floor((sqrt(8*n+1)+1)/2) */

/* Required helper for binomial coefficients. */
#include "../utils/binom.h"

/* Integer square root for idx_t (uint64_t) values. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v >> 1U;
    if (v >= 3) hi = (hi + 1U) * 2U;          /* quick upper bound */
    idx_t mid;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        if (mid * mid <= v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* safety; the sequence is defined for n≥1 */

    /* compute b = floor((sqrt(8*n+1)+1)/2) */
    idx_t t   = 8 * n + 1;
    idx_t s   = isqrt_u64(t);
    idx_t b   = (s + 1) / 2;                    /* b is still an idx_t */

    seq_t bin3 = binom(b, 3);                  /* C(b,3) */
    seq_t bin2 = binom(b, 2);                  /* C(b,2) */

    seq_t term1 = bin3;

    /* term2 = (n - bin2) * (b*b + 3*b - 2*(n+1)) / 4 */
    seq_t bb   = (seq_t)b * (seq_t)b;
    seq_t part = bb + 3 * (seq_t)b - 2 * ((seq_t)n + 1);
    seq_t diff = (seq_t)n - bin2;
    seq_t term2 = diff * part / 4;

    return term1 + term2;
}
