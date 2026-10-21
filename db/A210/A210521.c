/*
 * A210521  Array read by downward antidiagonals: T(n,k) = (n+k-1)*(n+k-2) + n
 *          + floor((n+k)/2)*(1-2*floor((n+k)/2)), for n, k > 0.
 * https://oeis.org/A210521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where * i = 1 + (n - (s-2)(s-1)/2 - 1) * j = s - i
 *            * s is the smallest integer such that (s-1)*s/2 >= n * T(i,j) =
 *            (i+j-1)*(i+j-2) + 2*i + v*(1-2*v) * with v = floor((i+j)/2) *
 *            This implements the OEIS sequence A210521
 * Range:     u8 n <= 243, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* trivial cases: n must be >= 1 according to the offset */
    if (n == 0) return (seq_t)0;  /* undefined, but guard for safety */

    idx_t s = 2;                /* diagonal sum (i + j) starts from 2 */
    idx_t cum = 1;              /* cumulative count of terms up to this diagonal */

    while (cum < n) {
        ++s;
        cum += s - 1;           /* length of this diagonal is (s-1) */
    }

    /* compute offset inside diagonal (0‑based) */
    idx_t prev_cum = cum - (s - 1);
    idx_t offset = n - prev_cum - 1;

    idx_t i = offset + 1;         /* row index */
    idx_t j = s - i;              /* column index */

    idx_t sum = i + j;            /* i + j = s */
    seq_t v = (seq_t)(sum / 2);

    seq_t term = (seq_t)(sum - 1) * (sum - 2)        /* (i+j-1)*(i+j-2) */
               + (seq_t)2 * (seq_t)i                  /* + 2*i */
               + v * (1 - 2 * v);                    /* + v*(1-2*v) */

    return term;
}
