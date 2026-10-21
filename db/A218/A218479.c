/*
 * A218479  Number of 3n-length 9-ary words, either empty or beginning with
 *          the first letter of the alphabet, that can be built by repeatedly
 *          inserting triples of identical letters into the initially empty
 *          word.
 * https://oeis.org/A218479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{j=0..n-1} C(3n, j) * (n-j) * 8^j (OEIS
 *            A218479)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Pre‑compute 3*n */
    idx_t three_n = n * (idx_t)3;

    seq_t sum   = (seq_t)0;  /* Σ term */
    seq_t binom = (seq_t)1;  /* C(3n, 0) */
    seq_t pow8  = (seq_t)1;  /* 8^0 */

    for (idx_t j = (idx_t)0; j < n; ++j)
    {
        /* term = C(3n, j) * (n-j) * 8^j */
        sum += binom * (seq_t)(n - j) * pow8;

        /* Prepare next binomial coefficient
           C(3n, j+1) = C(3n, j) * (3n - j) / (j+1) */
        binom = binom * (seq_t)(three_n - j) / (seq_t)(j + (idx_t)1);

        /* next power of 8 */
        pow8 *= (seq_t)8;
    }

    /* Final division by n (exact integer division) */
    return sum / (seq_t)n;
}
