/*
 * A038248  Triangle whose (i,j)-th entry is binomial(i,j)*5^(i-j)*6^j.
 * https://oeis.org/A038248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i, j) * 5^(i-j) * 6^j, where the index n
 *            enumerates the triangle row‑wise (i = 0,1,2,…; j = 0…i)
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Determine row i and column j from n by subtracting row lengths.
       After the loop: i is the row, j = n - i*(i+1)/2. */
    idx_t i = 0;
    idx_t rem = n;
    while (rem >= i + 1) {
        rem -= i + 1;
        ++i;
    }
    idx_t j = rem;

    /* Compute the binomial coefficient binomial(i, j). */
    seq_t val = binomial(i, j);

    /* Multiply by 5^(i-j). */
    if (i > j) {
        val = seq_mul(val, seq_pow((seq_t)5, i - j));
    }

    /* Multiply by 6^j. */
    if (j > 0) {
        val = seq_mul(val, seq_pow((seq_t)6, j));
    }

    return val;
}
