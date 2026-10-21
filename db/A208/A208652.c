/*
 * A208652  Product{i*C(n,i) : 1<=i<=floor[(n+1)/2]}.
 * https://oeis.org/A208652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product{i*C(n,i) : 1<=i<=floor[(n+1)/2]}
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"  /* for seq_gcd */

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t limit = (n + 1) / 2;      /* floor[(n+1)/2] */
    seq_t product = (seq_t)1;       /* accumulator */
    seq_t binom   = (seq_t)n;       /* C(n,1) */

    for (idx_t i = 1; i <= limit; ++i) {
        seq_t ii   = (seq_t)i;
        seq_t term = ii * binom;    /* i * C(n,i) */
        product = product * term;   /* accumulate product */

        if (i < limit) {
            /* Update binom to C(n,i+1) = C(n,i) * (n-i) / (i+1) */
            seq_t numerator   = binom * (n - i);
            seq_t denominator = (seq_t)(i + 1);
            binom = numerator / denominator;
        }
    }
    return product;
}
