/*
 * A137737  Number of circular permutations of the multiset {1,1,2,2,...,n,n}
 *          (up to rotations) with even distances between equal elements.
 * https://oeis.org/A137737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for odd n a(n) = ((n!^3)/((n/2)!^2 * 2^n)+n!)/(2n) for
 *            even n (equivalently a(2k) = ( (2k)! * ((2k-1)!!)^2 + (2k)! ) /
 *            (4k) ) where (2k-1)!! = product of the odd integers ≤ 2k-1
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 9, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if ((n & 1u) != 0u)          /* odd n -> 0 */
        return 0;

    idx_t half = n >> 1;          /* k = n/2 */
    seq_t fact_n = factorial(n);   /* (2k)! */

    /* compute product of odd integers up to 2k-1 = n-1 */
    seq_t prod_odd = 1;
    for (idx_t i = 1; i <= n; i += 2)
        prod_odd *= (seq_t)i;

    /* C = (2k-1)!! ; compute C^2 */
    seq_t c_sq = prod_odd;
    c_sq *= prod_odd;              /* prod_odd * prod_odd */

    /* term = fact_n * C^2 */
    seq_t term = fact_n;
    term *= c_sq;

    /* sum = term + fact_n = (2k)! * C^2 + (2k)! */
    seq_t sum = term + fact_n;

    /* denom = 2n */
    seq_t denom = (seq_t)(2 * n);

    return sum / denom;
}
