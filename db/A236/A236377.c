/*
 * A236377  Real part of Sum_{k=0..n} (k + i^k)^2, where i=sqrt(-1).
 * https://oeis.org/A236377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{k=0..n} (k + i^k)^2.re
 *          where i = sqrt(-1), and the real part is
 *          k^2 + 1 + 2k * (-1)^{k/2}   if k even
 *          k^2 - 1                      if k odd
 */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;          /* k as seq_t */
        seq_t k_sq  = k_seq * k_seq;     /* k^2 */

        if ((k & 1) == 0) {             /* k even */
            seq_t term = k_sq + (seq_t)1;                     /* k^2 + 1 */
            seq_t t2k  = k_seq + k_seq;                       /* 2k */
            if ((k >> 1) & 1) {                                 /* m odd -> subtract 2k */
                sum += term - t2k;
            } else {                                            /* m even -> add 2k */
                sum += term + t2k;
            }
        } else {                         /* k odd */
            sum += k_sq - (seq_t)1;
        }
    }
    return sum;
}
