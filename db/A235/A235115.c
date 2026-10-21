/*
 * A235115  Number of independent vertex subsets of the graph obtained by
 *          attaching two pendant edges to each vertex of the star graph S_n
 *          (having n vertices; see A235114).
 * https://oeis.org/A235115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A235115
   Formula: a(n) = 4*5^(n-1) + 2^(2*n-2)   for n >= 1
   Recurrence: a(n) = 9*a(n-1) - 20*a(n-2)   for n > 2  (not used here)
*/

#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */
#include "../utils/pow2.h"      /* pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* compute 4 * 5^(n-1) */
    seq_t term1 = 4 * big_pow((seq_t)5, (seq_t)(n - 1));

    /* compute 2^(2*n-2) */
    seq_t term2 = pow2(2 * n - 2);

    /* sum both terms */
    return term1 + term2;
}
