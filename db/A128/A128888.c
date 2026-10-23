/*
 * A128888  Table with g.f. [1-x*n-sqrt(x^2*n^2-2*n*x+1+4*x^2-4*x)]/(2*x).
 * https://oeis.org/A128888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   the generating function for a(n,m) is a(n,m) = coefficient of
 *            x^m in [1 - n x - sqrt(1 - 2(n+2)x + (n^2+4)x^2)]/(2x) The
 *            sequence a(k) requested is the diagonal read of the table a(n,m)
 *            Since the n-th output is the element whose row+column = s with
 *            s(s+1)/2 <= n < (s+1)(s+2)/2, and inside that an...
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static inline idx_t diagonal_index(idx_t n, idx_t *row, idx_t *col)
{
    /* Find smallest s such that (s+1)(s+2)/2 > n */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;
    idx_t t = n - s * (s + 1) / 2;
    *row = s - t;
    *col = t;
    return s;
}

static inline seq_t compute_a_row_col(idx_t r, idx_t c)
{
    if (c == 0) return 1;        /* a_{r,0} = 1 */

    /* Use signed 128-bit for intermediate arithmetic */
    __int128 ci = r;             /* signed conversion of r */
    __int128 c0 = 1;             /* coefficient c_0 of sqrt expansion */
    __int128 c1 = - (ci + 2);    /* coefficient c_1 = -(r+2) */
    /* store coefficients up to index c_index = c+1 */
    size_t size = (size_t)(c + 2);
    __int128 *carr = NULL;
    carr = (__int128 *)malloc(size * sizeof(__int128));
    if (!carr) return 0;          /* allocation failure -> 0 */
    carr[0] = c0;
    carr[1] = c1;

    /* coefficient c_2 */
    __int128 A = ci * ci + 4;     /* n^2 + 4 (positive) */
    carr[2] = (A - c1 * c1) / 2;

    /* Recurrence for k >= 3 */
    for (size_t k = 3; k <= (size_t)(c + 1); ++k) {
        __int128 sum = 0;
        for (size_t i = 1; i < k; ++i) {
            sum += carr[i] * carr[k - i];
        }
        carr[k] = - sum / 2;
    }

    /* a_{r,c} = -c_{c+1}/2 */
    __int128 val = -carr[c + 1] / 2;
    free(carr);
    return (seq_t) val;
}

seq_t a(idx_t n)
{
    idx_t r, c;
    diagonal_index(n, &r, &c);
    return compute_a_row_col(r, c);
}
