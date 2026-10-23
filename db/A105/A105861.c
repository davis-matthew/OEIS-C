/*
 * A105861  a(n) = (n/2) * Sum_{k=0..n} binomial(n,k)/gcd(n,k).
 * https://oeis.org/A105861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n/2) * Σ_{k=0..n} binomial(n,k) / gcd(n,k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute gcd of two idx_t values */
static inline idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n == 0 we return 0. */
    if (n == 0) return 0;

    seq_t sum   = 0;
    seq_t binom = 1;            /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* n/gcd(n,k) fits in idx_t, convert to seq_t for the product */
        idx_t g   = gcd_uint64(n, k);
        seq_t mul = (seq_t)(n / g);

        sum += binom * mul;

        if (k == n) break;     /* last term processed */

        /* Update binomial coefficient to C(n,k+1) */
        binom = binom * (seq_t)(n - k);
        binom = binom / (seq_t)(k + 1);
    }

    /* The sum is always even; divide by 2 to get the required result */
    return sum / 2;
}
