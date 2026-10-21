/*
 * A081207  Main diagonal of number square A081206.
 * https://oeis.org/A081207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (binomial(floor((n+k)/2), k))^2
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* a(n) – Main diagonal of number square A081206 (OEIS A081207) */
seq_t a(idx_t n)
{
    /* Handle the trivial cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)7;

    seq_t sum = (seq_t)0;
    /* initial binomial (k = 0) */
    seq_t bin = (seq_t)1;
    idx_t prevM = n / 2;          /* floor((n+0)/2) */
    sum += bin * bin;             /* add the square of the first term */

    /* Compute the remaining terms. */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = (n + k) / 2;    /* floor((n+k)/2) */

        if (m == prevM) {
            /* Use the recurrence C(m,k) = C(m,k-1) * (m-k+1) / k. */
            seq_t num = (seq_t)(m - k + 1);
            seq_t den = (seq_t)k;
            bin = (bin * num) / den;
        } else {
            /* m has increased, compute the binomial from scratch. */
            bin = binomial(m, k);
        }

        sum += bin * bin;        /* add the square of the current binomial */
        prevM = m;
    }

    return sum;
}
