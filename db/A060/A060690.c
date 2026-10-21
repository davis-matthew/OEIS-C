/*
 * A060690  a(n) = binomial(2^n + n - 1, n).
 * https://oeis.org/A060690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2^n + n - 1, n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t) */
#include "../utils/gcd.h"    /* provides seq_t gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* Base case: n = 0  ->  binomial(1 - 1, 0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* N = 2^n + n - 1  (may be big for arbitrary-precision seq_t) */
    seq_t two_pow = pow2(n);                 /* 2^n */
    seq_t N = two_pow + (seq_t)n - (seq_t)1; /* 2^n + n - 1 */

    seq_t result = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Numerator of the k‑th term: N - (n - k) = 2^n - 1 + k */
        seq_t num   = N - (seq_t)(n - k);
        idx_t   den = k;            /* denominator (small) */

        /* Cancel common factors between result and den */
        seq_t g1 = gcd(result, (seq_t)den);
        result  /= g1;
        den      = (idx_t)(den / (uint64_t)g1);

        /* Cancel common factors between num and den */
        seq_t g2 = gcd(num, (seq_t)den);
        num     /= g2;
        den      = (idx_t)(den / (uint64_t)g2);

        /* After the above cancellations den should be 1 for an integer binomial */
        /* the product result * num now equals binomial(2^n + n - 1, k) */
        result = result * num;
    }

    return result;
}
