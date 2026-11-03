/*
 * A127571  Triangle T(n,k) = phi(n/k)*sigma(k) if k divides n, else 0.
 * https://oeis.org/A127571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where row and col satisfy * n = row*(row-1)/2
 *            + col, 1 <= col <= row * T(row,col) = phi(row/col)*sigma(col) if
 *            col | row, else 0
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   isqrt, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/isqrt.h"
#include "../utils/totient_seq.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard for safety */

    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = (isqrt(1 + 8 * n) - 1) / 2;   /* floor */
    if (r * (r + 1) / 2 < n) r++;              /* ceil if needed */

    /* Previous row ends at r*(r-1)/2 */
    idx_t prev = r * (r - 1) / 2;
    idx_t col  = n - prev;                     /* 1 <= col <= r */

    if (r % col != 0)          /* not a divisor, entry is zero */
        return 0;

    idx_t div = r / col;                       /* integer division */

    seq_t phi_val  = totient_seq((seq_t)div);
    seq_t sigma_val = sigma_of(col);
    return phi_val * sigma_val;
}
