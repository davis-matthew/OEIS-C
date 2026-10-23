/*
 * A010930  Binomial coefficient C(14,n).
 * https://oeis.org/A010930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(14, n)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Return binomial coefficient C(14, n). For n > 14, the value is 0. */
    if (n > 14)
        return (seq_t)0;

    seq_t res = 1;
    idx_t k = n;
    if (k > 14 - k)
        k = 14 - k;   /* symmetry property */

    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply then divide to keep intermediate values integral. */
        seq_t numerator = (seq_t)(14 - k + i);
        seq_t denominator = (seq_t)i;
        res = (res * numerator) / denominator;
    }
    return res;
}
