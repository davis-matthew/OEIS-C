/*
 * A186323  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the squares and heptagonal
 *          numbers. Complement of A186322.
 * https://oeis.org/A186323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(H_n - 1))
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   floor_seq_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where H_n = n*(5*n - 3)/2 is the n‑th heptagonal number   */
/* The rank of H_n in the merged sorted list of squares and   */
/* heptagonals (heptagons first when equal) is n plus the number  */
/* of squares strictly less than H_n.                         */

#include <stdint.h>
#include "../utils/floor_seq_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t seq_n = (seq_t)n;

    /* Compute H_n = n * (5*n - 3) / 2 */
    seq_t five_n        = seq_n * ((seq_t)5);
    seq_t five_n_minus3 = five_n - ((seq_t)3);
    seq_t H             = (seq_n * five_n_minus3) / ((seq_t)2);

    /* Compute floor(sqrt(H - 1)) */
    seq_t H_minus_1 = H - ((seq_t)1);
    seq_t sq        = floor_seq_sqrt(H_minus_1);

    /* The required rank is n + sq */
    return seq_n + sq;
}
