/*
 * A219564  Sum(binomial(n+k,k)^6, k=0..n).
 * https://oeis.org/A219564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n + k, k)^6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"

/* Compute the n-th term of OEIS sequence A219564 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;                      /* C(n+0,0) = 1 */
    seq_t n_seq = (seq_t)n;

    /* Add the k = 0 term */
    sum += seq_pow(bin, 6);

    for (idx_t k = 1; k <= n; ++k)     /* loop for k = 1 .. n */
    {
        seq_t num = n_seq + (seq_t)k;  /* n + k   */
        seq_t den = (seq_t)k;          /* k       */

        /* Reduce the fraction (bin * num / den) to avoid overflow */
        seq_t g1 = seq_gcd(bin, den);
        seq_t bin1 = bin / g1;
        seq_t den1 = den / g1;

        seq_t g2 = seq_gcd(num, den1);
        seq_t num1 = num / g2;
        /* den1 / g2 should be 1 now */

        bin = bin1 * num1;             /* Next binomial coefficient */

        sum += seq_pow(bin, 6);        /* Add term to the sum */
    }

    return sum;
}
