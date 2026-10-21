/*
 * A098054  Absolute values of elements of flattened successive matrices B(k),
 *          k>=0, read by rows where B(k) = M^k*C where M=[[2, -1], [-1, 1]]
 *          and C = [[0, 1], [1, 1]].
 * https://oeis.org/A098054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 51, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   signed_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Let B(0) = [[0,1],[1,1]].
   For k ≥ 0: B(k+1) = M · B(k) where M = [[2,-1],[-1,1]].
   Flatten B(k) by rows into (a_k,b_k,c_k,d_k);  
   a(n) is the absolute value of the integer at position n in the
   concatenation of all these rows.
*/

#include <stdbool.h>
#include "../utils/signed_add.h"

seq_t a(idx_t n)
{
    /* Matrix elements represented by magnitude and sign separately.
       |value| is stored in the magnitude; the sign bit indicates
       whether the actual value is positive (0) or negative (1). */
    seq_t a_m = 0, b_m = 1, c_m = 1, d_m = 1;      /* magnitudes */
    bool   a_s = false, b_s = false, c_s = false, d_s = false; /* signs */

    idx_t term = 0;
    while (1) {
        /* term 0: a_k   */
        if (term == n) return a_m;
        ++term;
        /* term 1: b_k   */
        if (term == n) return b_m;
        ++term;
        /* term 2: c_k   */
        if (term == n) return c_m;
        ++term;
        /* term 3: d_k   */
        if (term == n) return d_m;
        ++term;

        /* store old values before updating */
        seq_t old_a_m = a_m, old_b_m = b_m, old_c_m = c_m, old_d_m = d_m;
        bool   old_a_s = a_s, old_b_s = b_s, old_c_s = c_s, old_d_s = d_s;

        /* a_{k+1} = 2*a_k - c_k  */
        seq_t two_a_m = old_a_m * 2;
        signed_add(two_a_m, old_a_s, old_c_m, !old_c_s, &a_m, &a_s);

        /* b_{k+1} = 2*b_k - d_k  */
        seq_t two_b_m = old_b_m * 2;
        signed_add(two_b_m, old_b_s, old_d_m, !old_d_s, &b_m, &b_s);

        /* c_{k+1} = -a_k + c_k  */
        signed_add(old_a_m, !old_a_s, old_c_m, old_c_s, &c_m, &c_s);

        /* d_{k+1} = -b_k + d_k  */
        signed_add(old_b_m, !old_b_s, old_d_m, old_d_s, &d_m, &d_s);
    }
}
