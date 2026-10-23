/*
 * A038245  Triangle whose (i,j)-th entry is binomial(i,j)*5^(i-j)*3^j.
 * https://oeis.org/A038245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*5^(i-j)*3^j where i =
 *            floor((sqrt(8n+1)-1)/2) j = n - i*(i+1)/2 This folds the OEIS
 *            triangle T(i,j)=binomial(i,j)*5^(i-j)*3^j into a one‑dimensional
 *            sequence
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial, pow3, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/power5.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Determine row i and column j from the flat index n. */
    idx_t i = 0;
    /* i is the unique integer such that i*(i+1)/2 <= n < (i+1)*(i+2)/2 */
    while ((i + 1ULL) * (i + 2ULL) / 2ULL <= n) {
        ++i;
    }
    idx_t j = n - i * (i + 1ULL) / 2ULL;

    /* Compute binomial coefficient and the powers of 5 and 3. */
    seq_t bin = binomial(i, j);
    seq_t pow5_val = power5(i - j);   /* 5^(i-j) */
    seq_t pow3_val = pow3(j);          /* 3^j     */

    /* Multiply all parts together. */
    seq_t result = bin;
    result *= pow5_val;
    result *= pow3_val;

    return result;
}
