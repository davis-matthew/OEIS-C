/*
 * A127905  Construct triangle in which n-th row is obtained by expanding
 *          (1+x+x^3)^n and take the next-to-central column.
 * https://oeis.org/A127905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * Σ_{k=0..⌊(n‑1)/3⌋} binomial(n-1,3k) * binomial(3k,k)
 *            / (2k+1) (binomial uses the utility function binom from
 *            ../utils/binom.h)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0U) {
        return (seq_t)0;
    }

    idx_t m = n - 1U;        /* n-1 */
    idx_t maxk = m / 3U;     /* floor((n-1)/3) */
    seq_t sum = (seq_t)0;

    for (idx_t k = 0U; k <= maxk; ++k) {
        idx_t t = 3U * k;                       /* 3k   */
        seq_t term = binom(m, t);                /* C(n-1,3k)   */
        term = term * binom(t, k);              /* * C(3k,k)    */
        term = term / (seq_t)(2U * k + 1U);      /* / (2k+1)     */
        sum += term;
    }

    return sum * (seq_t)n;
}
