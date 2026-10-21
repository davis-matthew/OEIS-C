/*
 * A221216  T(n,k) = ((n+k)^2-2*(n+k)+4-(3*n+k-2)*(-1)^(n+k))/2; n , k > 0,
 *          read by antidiagonals.
 * https://oeis.org/A221216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((i+j)^2-2*(i+j)+4-(3*i+j-2)*(-1)^(i+j))/2 where (i,j)
 *            are obtained from the anti‑diagonal enumeration of the
 *            two‑dimensional table T(k,l). Index n starts at 1
 * Range:     u8 n <= 231, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal s such that cumulative counts >= n */
    idx_t s = 1;
    while ((s - 1) * s / 2 < n) s++;

    /* Number of entries before this diagonal */
    idx_t prev = ((s - 2) * (s - 1)) / 2;

    /* Position inside the diagonal (1‑based) */
    idx_t offset = n - prev;

    /* Convert to table coordinates (i,j) */
    idx_t i = offset;          /* row */
    idx_t j = s - i;           /* column */

    /* Compute the pairing function value using the given formula */
    seq_t ni = (seq_t)i;
    seq_t nj = (seq_t)j;
    seq_t sum = ni + nj;                              /* i + j */
    seq_t t   = sum * sum - 2 * sum + 4;              /* (i+j)^2-2(i+j)+4 */
    seq_t sub = 3 * ni + nj - 2;                      /* 3*i + j - 2*/

    seq_t result;
    if ((sum & 1) == 0) {                             /* (-1)^(i+j) = +1 */
        result = (t - sub) / 2;
    } else {                                          /* (-1)^(i+j) = -1 */
        result = (t + sub) / 2;
    }
    return result;
}
