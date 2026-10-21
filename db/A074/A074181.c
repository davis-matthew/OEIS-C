/*
 * A074181  Smallest power of n >= n!.
 * https://oeis.org/A074181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^ceiling(log_n(n!))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow_seq.h"

/*
 * Compute the smallest power of n that is at least n!.
 * Uses the identity a(n) = n^ceil(log(n!)/log(n)).
 */
seq_t a(idx_t n)
{
    /* Base case: a(1) = 1 */
    if (n <= 1) {
        return (seq_t)1;
    }

    /* Compute sum_{i=1}^{n} log(i) using long double for precision */
    long double ln_n = logl((long double)n);
    long double sum = 0.0L;
    for (idx_t i = 2; i <= n; ++i) {
        sum += logl((long double)i);
    }

    /* Ratio of logarithms */
    long double ratio = sum / ln_n;

    /* Small epsilon to guard against floating‑point rounding errors near an integer */
    const long double eps = 1e-12L;
    idx_t exponent = (idx_t)ceill(ratio - eps);

    /* Return n raised to the computed exponent */
    return pow_seq((seq_t)n, exponent);
}
