/*
 * A100140  Largest denominator of greedy Egyptian fraction sum for m/n, 1 <=
 *          m <= n-1.
 * https://oeis.org/A100140
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest denominator appearing in the greedy Egyptian
 *            fraction expansion of any m/n with 1 <= m <= n-1
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 30, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_gcd.h"

/* Calculate a(n) for n >= 2. */
seq_t a(idx_t n)
{
    seq_t max_denom = (seq_t)0;
    for (idx_t m = (idx_t)1; m < n; ++m) {
        /* start with fraction m/n */
        seq_t num = (seq_t)m;
        seq_t den = (seq_t)n;

        while (num != (seq_t)0) {
            /* k = ceil(den / num) */
            seq_t k = (den + num - (seq_t)1) / num;
            if (k > max_denom) max_denom = k;

            /* compute next remainder num/den */
            seq_t new_num = num * k - den;
            if (new_num == (seq_t)0)
                break;   /* remainder zero, end of expansion */

            seq_t new_den = den * k;

            /* reduce the fraction */
            seq_t g = seq_gcd(new_num, new_den);
            num = new_num / g;
            den = new_den / g;
        }
    }
    return max_denom;
}
