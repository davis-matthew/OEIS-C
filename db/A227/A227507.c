/*
 * A227507  Table of p(a,n) read by antidiagonals, where p(a,n) = Sum_{k=1..n}
 *          gcd(k,n) exp(2 Pi i k a / n) is the Fourier transform of the
 *          greatest common divisor.
 * https://oeis.org/A227507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, tabl
 * Formula:   a(n) = p(a,n) read by antidiagonals according to OEIS A227507
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Determine the diagonal (k) and position (a) within that diagonal.
       For diagonal k, there are k entries indexed by a = 0 .. k-1 with
       column index col = k - a. The cumulative number of entries
       up to diagonal k is k*(k+1)/2. */

    /* Find minimal k such that k*(k+1)/2 >= n. */
    idx_t k = (idx_t)((sqrtl((long double)8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L);
    /* Correct for any rounding errors. */
    while ((unsigned __int128)k * (k + 1) / 2 < n) ++k;
    while (k > 0 && (unsigned __int128)(k - 1) * k / 2 >= n) --k;

    /* Number of entries before this diagonal. */
    idx_t prev = (k - 1) * k / 2;
    /* 0‑based offset in the current diagonal. */
    idx_t pos = n - prev - 1;   /* pos in [0, k-1] */

    /* Coordinates (a, col). */
    idx_t a_coord = pos;
    idx_t col = k - pos;      /* col >= 1 */

    /* Compute p(a_coord, col). */
    idx_t g = (idx_t)gcd((seq_t)a_coord, (seq_t)col);  /* gcd as idx_t */

    seq_t result = 0;
    /* Sum over all divisors d of g: d * phi(col/d). */
    for (idx_t d = 1; d <= g; ++d) {
        if (g % d == 0) {
            idx_t quot = col / d;
            seq_t phi_val = phi(quot);
            result += (seq_t)d * phi_val;
        }
    }
    return result;
}
