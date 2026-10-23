/*
 * A010986  Binomial coefficient C(n,33).
 * https://oeis.org/A010986
 *
 * Offset:    33    (a(33) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,33)
 * Range:     u8 n <= 34, u16 n <= 36, u32 n <= 43, u64 n <= 67, u128 n <= 209,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Return C(n,33). Valid for n >= 33 (other values are unused). */
    if (n < 33) return 0;          /* behaviour below offset is irrelevant */

    const idx_t k = 33;
    seq_t result = 1;

    for (idx_t i = 1; i <= k; ++i) {
        seq_t numer = (seq_t)(n - k + i);   /* numerator term */
        seq_t denom = (seq_t)i;             /* denominator term */

        seq_t g = gcd(numer, denom);      /* cancel gcd between num and denom */
        numer /= g;
        denom /= g;

        g = gcd(result, denom);           /* cancel any remaining factor from result */
        result /= g;
        denom /= g;

        result = result * numer;          /* multiply by reduced numerator */

        /* after cancellation denom should be 1; otherwise the algorithm fails */
    }

    return result;
}
