/*
 * A074638  Denominator of 1/3 + 1/7 + 1/11 + ... + 1/(4n-1).
 * https://oeis.org/A074638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   seq_gcd_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd_seq_t.h"
/* Formula for A074638: a(n) = denominator of ∑_{j=0}^{n-1} 1/(4j+3) */

seq_t a(idx_t n)
{
    /* Handle offset: a(1) is first term. For n<1 behavior is irrelevant. */
    seq_t num = 0;          /* numerator of the running sum */
    seq_t denom = 1;        /* denominator of the running sum */

    for (idx_t j = 0; j < n; ++j)
    {
        seq_t d = (seq_t)4 * j + (seq_t)3;   /* denominator of current term */

        /* Add 1/d to num/denom: (num* d + denom) / (denom * d) */
        seq_t new_num = num * d + denom;
        seq_t new_den = denom * d;

        /* Reduce by gcd to keep numbers as small as possible */
        seq_t g = seq_gcd_seq_t(new_num, new_den);
        num = new_num / g;
        denom = new_den / g;
    }

    return denom;
}
