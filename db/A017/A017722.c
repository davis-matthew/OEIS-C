/*
 * A017722  Binomial coefficients C(n,58).
 * https://oeis.org/A017722
 *
 * Offset:    58    (a(58) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,58)
 * Range:     u8 n <= 59, u16 n <= 61, u32 n <= 65, u64 n <= 79, u128 n <= 133,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n ≥ 58 is required; below that the offset is not defined */
    if (n < 58)
        return (seq_t)0;

    const idx_t k = 58;
    idx_t offset = n - k;   /* n - 58, always non‑negative here */
    seq_t result = (seq_t)1;

    for (idx_t i = 1; i <= k; ++i) {
        seq_t a_num = (seq_t)(offset + i);   /* n - k + i */
        seq_t b_den = (seq_t)i;

        /* Reduce the fraction (a_num / b_den) */
        seq_t g = gcd(a_num, b_den);
        a_num /= g;
        b_den /= g;

        /* Reduce b_den with the current result */
        g = gcd(result, b_den);
        result /= g;
        b_den /= g;

        /* Now b_den should be 1; multiply the remaining numerator */
        result *= a_num;
    }

    return result;
}
