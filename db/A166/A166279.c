/*
 * A166279  Triangle, read by rows: T(0,0) = 1, T(n,k) = T(n-1,k-1) (mod 2) +
 *          T(n-1,k) (mod 2), T(n,k) = 0 if k < 0 or k > n.
 * https://oeis.org/A166279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = triangle entry T(r,k) where T(0,0) = 1 and T(n,k) =
 *            (T(n-1,k-1) % 2) + (T(n-1,k) % 2) read row‑wise. The value is 0,
 *            1 or 2
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   odd_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/odd_binom.h"

seq_t a(idx_t n)
{
    /* Determine the row r and column k for index n */
    double d = sqrt(8.0 * (long double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* Adjust r in case of rounding errors */
    if ((r + 1) * (r + 2) / 2 <= n) r++;
    while (r > 0 && r * (r + 1) / 2 > n) r--;

    idx_t prev_cum = r * (r + 1) / 2;   /* sum of entries in rows 0..r-1 */
    idx_t k = n - prev_cum;             /* 0 ≤ k ≤ r */

    if (r == 0) return (seq_t)1;          /* T(0,0) */

    seq_t val = 0;
    if (k > 0) {
        if (odd_binom(r - 1, k - 1)) val += 1;
    }
    if (k < r) {
        if (odd_binom(r - 1, k)) val += 1;
    }
    return val;
}
