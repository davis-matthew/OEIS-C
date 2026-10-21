/*
 * A062966  a(n) = C(3+n, n) + C(4+n, n) + C(5+n, n) + C(6+n, n).
 * https://oeis.org/A062966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+3,3) + C(n+4,4) + C(n+5,5) + C(n+6,6)
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 116, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom_n_r
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n_r.h"

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    sum += binom_n_r(n + (idx_t)3, (idx_t)3);  /* C(n+3,3) */
    sum += binom_n_r(n + (idx_t)4, (idx_t)4);  /* C(n+4,4) */
    sum += binom_n_r(n + (idx_t)5, (idx_t)5);  /* C(n+5,5) */
    sum += binom_n_r(n + (idx_t)6, (idx_t)6);  /* C(n+6,6) */
    return sum;
}
