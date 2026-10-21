/*
 * A221460  Number of 0..n arrays of length n with each element unequal to at
 *          least one neighbor, starting with 0.
 * https://oeis.org/A221460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A221460: number of arrays of length n over alphabet {0,…,n}
   starting with 0 where no element equals both its neighbors
   (i.e. no run of three identical elements and endpoints must differ from
   their single neighbor).  The DP below counts such arrays.
*/

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;                 /* a(1) = 0 by definition */

    /* k = alphabet size = n + 1 */
    seq_t k = (seq_t)n + 1;

    /* dpN: number of sequences ending with last two symbols unequal
       dpE: number of sequences ending with last two symbols equal    */
    seq_t dpN = k - 1;   /* at position 2 */
    seq_t dpE = 0;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t new_dpN = (k - 1) * (dpN + dpE);
        seq_t new_dpE = dpN;
        dpN = new_dpN;
        dpE = new_dpE;
    }

    return dpN;   /* only sequences ending with unequal neighbors are valid */
}
