/*
 * A203153  (n-1)-st elementary symmetric function of {2, 2, 3, 3, 4, 4, 5, 5,
 *          ..., floor((n+3)/2)}.
 * https://oeis.org/A203153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)-th elementary symmetric function of the multiset *
 *            {2,2,3,3,4,4,5,5,…, floor((n+3)/2)}, * with offset 1 (a(1)=1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the minimal case directly */
    if (n <= 1)
        return (seq_t)1;   /* a(1) = 1 */

    seq_t a_val = (seq_t)1;   /* a(1) */
    seq_t prod   = (seq_t)2;  /* product of the first element (2) */

    for (idx_t i = 2; i <= n; ++i) {
        /* Current element of the multiset */
        idx_t e = (i + 3) / 2;            /* floor((i+3)/2) */

        /* a(i) = e * a(i-1) + prod_{1..i-1} */
        a_val = (seq_t)e * a_val + prod;

        /* Update product for the next iteration */
        prod = prod * (seq_t)e;
    }

    return a_val;
}
