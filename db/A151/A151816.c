/*
 * A151816  a(n) = (2*n)! - ((2*n-1)!!)^2.
 * https://oeis.org/A151816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)! - ((2*n-1)!!)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t fact = 1;          /* (2*n)!            */
    seq_t dble_fact = 1;     /* (2*n-1)!!  (odd only) */

    idx_t limit = 2 * n;
    for (idx_t i = 1; i <= limit; ++i) {
        fact *= (seq_t)i;
        if (i & 1u)           /* i is odd */
            dble_fact *= (seq_t)i;
    }

    seq_t dbl_sq = dble_fact * dble_fact;
    return fact - dbl_sq;
}
