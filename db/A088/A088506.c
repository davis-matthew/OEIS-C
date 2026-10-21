/*
 * A088506  Number of permutations in the symmetric group S_n that have odd
 *          number of transpositions in their cycle decomposition.
 * https://oeis.org/A088506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - A088336(n) (OEIS A088506)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2u) return 0;          /* a(0) = a(1) = 0 */

    seq_t a_prev = 0;              /* a(1) */
    seq_t prod   = 0;              /* product for even i: ∏_{t=k+1}^{i} t , k = i/2 */

    for (idx_t i = 2; i <= n; ++i) {
        if (i & 1u) {                  /* i is odd: a(i) = i * a(i-1) */
            a_prev = (seq_t)i * a_prev;
        } else {                       /* i is even  */
            idx_t k = i >> 1;          /* k = i/2 */

            if (i == 2u) {             /* first even index */
                prod = (seq_t)i;        /* ∏_{t=2}^{2} t */
            } else {
                /* update prod from previous even index:
                   prod_new = prod_old / k * (i-1) * i   (exact division) */
                prod = prod / (seq_t)k * (seq_t)(i-1) * (seq_t)i;
            }

            seq_t term = prod / 2;      /* prod is always even */

            /* sign = (-1)^(k+1) */
            if ((k + 1u) & 1u) {
                a_prev = (seq_t)i * a_prev - term;   /* negative sign */
            } else {
                a_prev = (seq_t)i * a_prev + term;   /* positive sign */
            }
        }
    }
    return a_prev;
}
