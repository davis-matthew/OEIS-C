/*
 * A052512  Number of rooted labeled trees of height at most 2.
 * https://oeis.org/A052512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0}^{n-1} n * C(n-1,k) * (n-k-1)^k
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 38,
 *            big any n
 * Helpers:   binomial, pow_seq_mono
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */
#include "../utils/pow_seq_mono.h"

/* OEIS A052512: number of rooted labelled trees of height at most 2 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* a(0) = 0 by definition */

    seq_t sum = 0;
    const seq_t n_seq = (seq_t)n;      /* cast once for speed */

    for (idx_t k = 0; k <= n - 1; ++k)
    {
        seq_t comb  = binomial(n - 1, k);               /* C(n-1,k)   */
        seq_t base  = (seq_t)(n - k - 1);               /* n-k-1 */
        seq_t power = pow_seq_mono(base, k);            /* (n-k-1)^k  */
        seq_t term  = comb * power;                     /* comb * power */
        term *= n_seq;                                  /* * n          */
        sum += term;                                    /* accumulate    */
    }
    return sum;
}
