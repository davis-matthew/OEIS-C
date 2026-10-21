/*
 * A095886  Triangle read by rows: T(n,k) = (n-k)^n, n>=1, 1<=k<=n.
 * https://oeis.org/A095886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (r-k)^r * where r = ceil((sqrt(8*n+1)-1)/2) (the row
 *            containing n) * k = n - r(r-1)/2 (the column position in that
 *            row) * a(n) = (r-k)^r
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   pow_seq_inner
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/pow_seq_inner.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* values below the offset are irrelevant */
        return 0;

    /* find the row r that contains index n */
    double d  = sqrt(8.0 * (double)n + 1.0);
    idx_t r   = (idx_t)((d - 1.0) * 0.5);      /* floor of the solution */
    if (r * (r + 1) / 2 < n)
        ++r;   /* correct to the ceil if necessary */

    /* compute column k */
    idx_t prev = (r - 1) * r / 2;   /* cumulative count up to previous row */
    idx_t k    = n - prev;          /* 1‑based column index */

    /* compute base = r - k */
    seq_t base = (seq_t)(r - k);

    /* 0^exp is 0, 1^exp is 1 – fast‑path */
    if (base == 0)
        return 0;
    if (base == 1)
        return 1;

    return pow_seq_inner(base, r);
}
