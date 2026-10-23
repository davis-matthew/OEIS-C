/*
 * A119932  Triangle of denominators of the cube of a certain lower triangular
 *          matrix.
 * https://oeis.org/A119932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula:
   a(n) = denominator( (A^3)[i,j] ),
   where A is the lower‑triangular matrix with entries A[i,j] = 1/i for j≤i,
   and (i,j) is obtained by row–major numbering of the triangle.
*/

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Determine the row i and column j from the linear index n */
    idx_t i = 1;
    while ((i * (i + 1)) / 2 < n) ++i;
    idx_t j = n - ((i - 1) * i) / 2;   /* 1‑based column */

    /* Rational sum: maintain num/den and add each term 1/(i·k·l)   */
    seq_t num = 0;
    seq_t den = 1;
    seq_t i_seq = (seq_t)i;

    for (idx_t k = j; k <= i; ++k) {
        seq_t k_seq = (seq_t)k;
        for (idx_t l = j; l <= k; ++l) {
            seq_t l_seq = (seq_t)l;

            /* term denominator = i * k * l */
            seq_t term_den = i_seq * k_seq * l_seq;

            /* Common denominator of current sum and term */
            seq_t g = seq_gcd(den, term_den);
            seq_t den_div_g = den / g;              /* den/g  */
            seq_t l_den     = den_div_g * term_den; /* lcm(den, term_den) */

            /* Multipliers for the addition */
            seq_t num_l = term_den / g;    /* multipler for num  */
            seq_t den_l = den_div_g;        /* multiplies 1 (term numerator) */

            /* Add fractions: num/den + 1/term_den */
            seq_t new_num = num * num_l + den_l;
            seq_t new_den = l_den;

            /* Reduce the resulting fraction */
            seq_t g2 = seq_gcd(new_num, new_den);
            if (g2 != 1) {
                new_num = new_num / g2;
                new_den = new_den / g2;
            }

            num = new_num;
            den = new_den;
        }
    }

    /* The desired term is the denominator of the reduced fraction */
    return den;
}
