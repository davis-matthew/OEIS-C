/*
 * A221046  The Wiener index of the Bethe cactus lattice graph E_n defined
 *          pictorially in the Hosoya - Balasubramanian reference.
 * https://oeis.org/A221046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -(1/2)+3^n*(n+5)+3^(2*n)*(3*n-(9/2))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* for safety, ignore values below the offset */
    if (n < 1) return 0;

    /* t1 = 3^n, t2 = 3^(2n) = (3^n)^2 */
    seq_t t1 = big_pow(3, (seq_t)n);
    seq_t t2 = t1 * t1;

    /* compute terms without negative intermediate values */
    seq_t term1 = t1 * ((seq_t)(n + 5));
    term1 = term1 * 2;                  /* 2 * t1 * (n+5) */

    seq_t term2 = t2 * ((seq_t)6 * n);   /* 6*n * t2 */
    seq_t term3 = t2 * 9;                /* 9 * t2 */

    /* 2*a(n) = term1 + term2 - term3 - 1 */
    seq_t sum = term1 + term2;
    sum -= term3;
    sum -= 1;

    return sum / 2;
}
