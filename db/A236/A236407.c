/*
 * A236407  a(n) = 2*Sum_{k=0..n-1} C(n-1,k)*C(n+k,k) + n.
 * https://oeis.org/A236407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*∑_{k=0}^{n-1} C(n-1,k)*C(n+k,k) + n (offset 0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
typedef uint64_t idx_t;  /* assumed; actual type supplied by the build */

/* seq_t is supplied by the build; arithmetic operators are available */
seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0)
        return 0;

    seq_t sum = 0;
    seq_t c1 = 1; /* C(n-1,0) */
    seq_t c2 = 1; /* C(n+0,0) */

    for (idx_t k = 0; k < n; ++k) {
        sum += c1 * c2;          /* accumulate the summand */

        /* Update binomial coefficients for next k (avoid integer overflows) */
        if (k + 1 == n) /* last iteration; no need to update */
            break;

        seq_t denom = (seq_t)(k + 1);
        c1 = (c1 * (seq_t)(n - 1 - k)) / denom;   /* C(n-1, k+1) */
        c2 = (c2 * (seq_t)(n + k + 1)) / denom;   /* C(n+k+1, k+1) */
    }

    return sum * 2 + (seq_t)n;
}
