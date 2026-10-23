/*
 * A169961  a(n) = binomial(12*n, n).
 * https://oeis.org/A169961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(12*n, n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = (seq_t)1;
    seq_t total = (seq_t)(12 * n);   /* 12*n */

    for (idx_t i = 0; i < n; ++i) {
        seq_t num = total - (seq_t)i;          /* 12*n - i   */
        seq_t den = (seq_t)(i + 1);            /* i + 1      */

        /* cancel common factor between num and den */
        seq_t g1 = gcd(num, den);
        num /= g1;
        den /= g1;

        /* cancel common factor between result and remaining den */
        seq_t g2 = gcd(result, den);
        result /= g2;
        den /= g2;   /* should become 1 now */

        /* multiply by the reduced numerator */
        result *= num;
    }

    return result;
}
