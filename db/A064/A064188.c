/*
 * A064188  a(n) = Sum_{k=0..floor(n/2)} binomial(n - k*(k-1)/2, k).
 * https://oeis.org/A064188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n - k*(k-1)/2, k) The
 *            binomial is taken with the standard identity for negative upper
 *            arguments: binom(-m,k) = (-1)^k * binom(m + k - 1, k)
 * Range:     i8 n <= 10, i16 n <= 15, i32 n <= 21, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t binomial_general(idx_t k, seq_t m)
{
    /* Binomial with possibly negative upper argument m.  */
    if (m >= 0) {
        return binomial((idx_t)m, k);
    } else {
        seq_t pos   = (seq_t)(-m);          /* m < 0 => pos > 0 */
        seq_t val   = binomial((idx_t)(pos + k - 1), k);
        return (k & 1) ? -val : val;       /* (-1)^k * val */
    }
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = n / 2;
    for (idx_t k = 0; k <= limit; ++k) {
        seq_t kk = (seq_t)k;
        seq_t t  = kk * (kk - 1) / 2;     /* k*(k-1)/2  */
        seq_t m  = (seq_t)n - t;          /* n - k*(k-1)/2 */
        sum += binomial_general(k, m);
    }
    return sum;
}
