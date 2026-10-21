/*
 * A225497  Total number of rooted labeled trees over all forests on
 *          {1,2,...,n} in which one tree has been specially designated.
 * https://oeis.org/A225497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(5n-1)*(n+1)^(n-3) – for n=1,2 the negative exponent is
 *            treated as an exact division
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)               /* a(0) is not defined in OEIS notation */
        return 0;

    seq_t n_seq = (seq_t)n;                /* cast index to result type */
    seq_t base  = n_seq + 1;               /* (n+1) */
    /* numerator = n * (5n – 1)  */
    seq_t numerator = n_seq * (seq_t)(5 * (idx_t)n - 1);

    if (n >= 3) {
        seq_t pow_val = big_pow(base, (seq_t)(n - 3));
        return numerator * pow_val;
    } else {                            /* n = 1 or 2, use division */
        seq_t pow_den = big_pow(base, (seq_t)(3 - n));
        return numerator / pow_den;
    }
}
