/*
 * A145571  Numerators of partial sums for Liouville's constant.
 * https://oeis.org/A145571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(1)=1; for n≥2, a(n)=a(n-1)*10^(n!-(n-1)!)+1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   pow10, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n <= 1) {
        return seq_one();
    }

    seq_t a_prev = seq_one();  /* current a(i-1) */
    idx_t prev_fact = 1;       /* back up (i-1)! */
    for (idx_t i = 2; i <= n; ++i) {
        /* i! */
        idx_t curr_fact = prev_fact * i;
        /* z(i) = i! - (i-1)! */
        idx_t z = curr_fact - prev_fact;
        /* 10^z */
        seq_t pow_z = pow10(z);
        /* a(i) = a(i-1) * 10^z + 1 */
        a_prev = a_prev * pow_z + seq_one();
        /* update factorial for next step */
        prev_fact = curr_fact;
    }
    return a_prev;
}
