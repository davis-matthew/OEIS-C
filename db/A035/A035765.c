/*
 * A035765  Coordination sequence for C_28 lattice.
 * https://oeis.org/A035765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^28
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 12, u128 n <= 70,
 *            big any n
 * Helpers:   binom_outer, init_binom28
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   a(n) = Σ_{r=0}^{min(28,2n)} C(28,r) * C(27+2n-r, 27) .
   The inner binomial has a constant difference 27 between
   top and bottom, so it is C(M,27) with M=2n+27-r.
   We compute each C(M,27) multiplicatively; the sum has at most 29
   terms, so the cost per call is O(28·27) arithmetic operations,
   independent of n.
*/

#include <stdbool.h>
#include "../utils/binom_outer.h"
#include "../utils/init_binom28.h"

seq_t a(idx_t n) {
    static bool init = false;
    static seq_t binom28_arr[29];

    if (!init) {
        init_binom28(binom28_arr);
        init = true;
    }

    idx_t max_r = (n < 14 ? 2 * n : 28);            /* r runs 0..min(28,2n) */
    seq_t sum = 0;

    for (idx_t r = 0; r <= max_r; ++r) {
        idx_t M = 2 * n + 27 - r;                  /* M >= 27 because r <= 2n <= 28 */
        seq_t term = binom28_arr[r] * binom_outer(M);
        sum += term;
    }

    return sum;
}
