/*
 * A181545  G.f.: A(x) = Sum_{n>=0} (3n)!/(n!)^3 * x^(3n)/(1-x-x^2)^(3n+1).
 * https://oeis.org/A181545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n-k,k)^3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum  = 0;          /* running total of the sum */
    seq_t bin  = 1;          /* bin = binomial(n-k,k); starts at k=0 */
    idx_t  half = n / 2;     /* floor(n/2) */

    for (idx_t k = 0; ; ++k) {
        /* add bin^3 to the sum */
        seq_t tmp = bin * bin * bin;
        sum += tmp;

        if (k == half) break;   /* finished all required terms */

        /* compute binomial(n-(k+1),k+1) from binom(n-k,k) using
           the ratio: B_{k+1} = B_k * (n-2k)(n-2k-1) / ((n-k)(k+1))   */
        seq_t num = (seq_t)(n - 2*k) * (seq_t)(n - 2*k - 1);
        seq_t den = (seq_t)(n - k)   * (seq_t)(k + 1);
        bin = bin * num / den;
    }

    return sum;
}
