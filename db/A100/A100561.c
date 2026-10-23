/*
 * A100561  Denominator of Sum_{k=0..[n/2]} 1/binomial(n,k).
 * https://oeis.org/A100561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sum_{k=0..floor(n/2)} 1/binomial(n,k)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   my_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/my_gcd.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0. */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Current numerator/denominator of the running sum. */
    seq_t num = (seq_t)0;
    seq_t den = (seq_t)1;

    idx_t half = n / (idx_t)2;   /* floor(n/2) */

    seq_t binom = (seq_t)1;      /* binomial coefficient C(n,0) = 1 */

    for (idx_t k = (idx_t)0; k <= half; ++k) {
        if (k > (idx_t)0) {
            /* Update binom from C(n,k-1) to C(n,k):
               binom = binom * (n - (k-1)) / k
               All arithmetic uses seq_t, which is the same type as
               the result type. */
            binom = (binom * (seq_t)(n - (k - (idx_t)1))) / (seq_t)k;
        }

        /* Add the fraction 1/binom to the running sum. */
        seq_t g = my_gcd(den, binom);
        seq_t den_over_g = den / g;
        seq_t binom_over_g = binom / g;

        seq_t new_den = den_over_g * binom;       /* lcm(den, binom)   */
        seq_t new_num = num * binom_over_g + den_over_g;

        seq_t g2 = my_gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return den;
}
