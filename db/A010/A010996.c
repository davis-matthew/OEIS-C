/*
 * A010996  Binomial coefficient C(n,43).
 * https://oeis.org/A010996
 *
 * Offset:    43    (a(43) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,43) (binomial coefficient)
 * Range:     u8 n <= 44, u16 n <= 46, u32 n <= 52, u64 n <= 70, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for idx_t and seq_t types */

seq_t a(idx_t n)
{
    /* offset 43: return 0 for indices before the first defined term */
    if (n < 43)
        return (seq_t)0;

    /* k = 43 */
    idx_t const k = 43;
    /* store the numerator factors n-42 ... n */
    idx_t numerators[k];
    idx_t i;
    for (i = 0; i < k; ++i)
        numerators[i] = n - 42 + i;   /* 1,2,...,43 when n=43 */

    /* cancel the denominator factors 2...43 with the numerators */
    for (i = 2; i <= k; ++i) {
        idx_t d = i;
        idx_t p;
        for (p = 2; p <= d; ++p) {
            while (d % p == 0) {
                /* find a numerator divisible by p and cancel it */
                idx_t j;
                for (j = 0; j < k; ++j) {
                    if (numerators[j] % p == 0) {
                        numerators[j] /= p;
                        break;
                    }
                }
                d /= p;
            }
        }
    }

    /* multiply the remaining numerator factors to get the result */
    seq_t result = 1;
    for (i = 0; i < k; ++i)
        result *= (seq_t)numerators[i];

    return result;
}
