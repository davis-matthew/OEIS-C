/*
 * A065945  Bessel polynomial {y_n}''(2).
 * https://oeis.org/A065945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A065945:  Bessel polynomial y_n''(2)
   a(n) = sum_{k=2}^{n} (n+k)!/(k!(n-k)!) * k(k-1) / 4
*/

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* d_2 = n*(n-1)*(n+1)*(n+2) */
    seq_t d = (seq_t)n * (n - 1) * (n + 1) * (n + 2);
    seq_t sum = d;

    for (idx_t k = 3; k <= n; ++k) {
        seq_t num1  = (seq_t)(n + k);          /* n + k   */
        seq_t num2  = (seq_t)(n - k + 1);      /* n - k + 1 */
        seq_t denom = (seq_t)(k - 2);          /* k - 2   */

        /* Cancel common factors with denom */
        seq_t g = gcd(d, denom);
        d    /= g;
        denom /= g;

        g = gcd(num1, denom);
        num1 /= g;
        denom /= g;

        g = gcd(num2, denom);
        num2 /= g;
        denom /= g;   /* denom should now be 1 */

        /* Compute next term */
        d = (d * num1) * num2;   /* safe for 64‑bit builds because of prior cancellation */
        sum += d;
    }

    return sum / 4;
}
