/*
 * A042457  Denominators of continued fraction convergents to sqrt(756).
 * https://oeis.org/A042457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042457 – Denominators of continued‑fraction convergents to sqrt(756)
   Recurrence:
      a(0) = 1
      a(1) = 2
      for n >= 2:
          if n is odd   a(n) = 2  * a(n‑1) + a(n‑2)
          if n is even  a(n) = 54 * a(n‑1) + a(n‑2)
   The odd/even coefficients arise from the periodic continued‑fraction
   [27; 2,54,2,54,…] of sqrt(756).  */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prev2 = 0;   /* a(n-2)  */
    seq_t prev1 = 1;   /* a(n-1)  */
    if (n == 0) return 1;

    for (idx_t k = 1; k <= n; ++k) {
        unsigned int coef = (k & 1) ? 2 : 54;   /* odd:2, even:54 */
        seq_t cur = (seq_t)coef * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
