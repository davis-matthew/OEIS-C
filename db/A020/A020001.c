/*
 * A020001  Nearest integer to Gamma(n + 11/12)/Gamma(11/12).
 * https://oeis.org/A020001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(∏_{k=0}^{n-1} (k + 11/12))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For small seq_t types (uint8_t … uint64_t) use long double to avoid
       overflow of the intermediate product while still keeping the result
       exact for n where the rounded value fits in seq_t. */
    if (sizeof(seq_t) <= 8) {
        long double prod = 1.0L;
        for (idx_t i = 0; i < n; ++i) {
            prod *= (long double)i + 11.0L / 12.0L;
        }
        /* round to nearest integer (positive numbers only) */
        return (seq_t)(uint64_t)(prod + 0.5L);
    }

    /* For arbitrary‑precision seq_t values compute the rational product
       exactly and round at the end. */
    seq_t num  = 1;   /* numerator of the product */
    seq_t den  = 1;   /* denominator = 12^n */

    for (idx_t i = 0; i < n; ++i) {
        seq_t term_num = (seq_t)(12 * i + 11);  /* (12*i + 11) as seq_t */
        num *= term_num;
        den *= 12;
    }

    /* nearest integer: floor((num + den/2) / den) for positive values */
    seq_t half_den = den / 2;
    seq_t result   = (num + half_den) / den;
    return result;
}
