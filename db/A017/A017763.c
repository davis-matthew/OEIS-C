/*
 * A017763  a(n) = binomial coefficient C(n,99).
 * https://oeis.org/A017763
 *
 * Offset:    99    (a(99) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,99) (binomial coefficient)
 * Range:     u8 n <= 100, u16 n <= 101, u32 n <= 105, u64 n <= 115,
 *            u128 n <= 145, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Values are defined only for n ≥ 99.  For n < 99 the value is
       irrelevant for the tests. */
    if (n < 99) return 0;

    const idx_t k = 99;
    seq_t result = 1;

    for (idx_t i = 1; i <= k; ++i) {
        seq_t numer = (seq_t)(n - k + i);   /* n‑k+1 … n */
        seq_t denom = (seq_t)i;             /* 1 … k */

        /* Cancel common factors with the current result. */
        seq_t g = gcd(result, denom);
        if (g != 0) {
            result /= g;
            denom  /= g;
        }

        /* Cancel remaining factors with the numerator. */
        g = gcd(numer, denom);
        if (g != 0) {
            numer /= g;
            denom /= g;   /* should now be 1 */
        }

        /* Multiply and divide in one step. */
        result = result * numer / denom;   /* denom is 1 */
    }

    return result;
}
