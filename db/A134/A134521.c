/*
 * A134521  Triangle read by rows: T(n,k) = binomial(n,k) + (n + k) mod 2.
 * https://oeis.org/A134521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(row, col) + (row + col) mod 2 where row, col are
 *            determined by the row-major enumeration of the triangle T(n,k) =
 *            binomial(n,k) + (n + k) mod 2
 * Range:     u8 n <= 39, u16 n <= 182, u32 n <= 645, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>                     /* for sqrt */
#include "../utils/binom.h"           /* seq_t binom(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* Find the row r such that offset_row(r) = r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    double d = sqrt((double)(8 * n + 1));      /* 8n+1 is safely representable as double */
    idx_t r = (idx_t)((d - 1.0) / 2.0);        /* floor of (sqrt(8n+1)-1)/2 */

    /* Compute column k */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Compute the triangle entry and add parity correction. */
    seq_t val = binom(r, k);                   /* binomial coefficient as seq_t */
    if ( ((r + k) & 1) != 0 )                 /* (r + k) mod 2 */
        val += (seq_t)1;                       /* add 1 if odd */

    return val;
}
