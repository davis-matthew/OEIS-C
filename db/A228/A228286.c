/*
 * A228286  Smallest x + y*z, given x*y + z = n (for positive integers x, y,
 *          z).
 * https://oeis.org/A228286
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1527, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A228286:  a(n) = min_{x,y,z >= 1, xy + z = n} (x + y*z)
   The minimum is achieved by choosing y ≥ 2 and the maximum possible x for that y.
   For each y ≥ 2, let x_max = ⌊(n-1)/y⌋ (the largest x with xy ≤ n-1).
   Then z = n - x_max*y and the value is val = x_max + y*z.
   The minimal value over all y and the trivial y = 1 case (val = n) gives a(n).
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) {
        /* sequence is defined for n ≥ 2; the value for n < 2 is unspecified */
        return (seq_t)0;
    }

    /* y = 1 gives value n, serve as initial upper bound */
    seq_t best = (seq_t)n;

    for (idx_t y = 2; y < n; ++y) {
        idx_t x_max = (n - 1) / y;          /* largest x with xy ≤ n-1  */
        if (x_max == 0)                    /* no valid x for larger y    */
            break;

        idx_t z      = n - x_max * y;       /* z = n - xy                 */
        idx_t val_tmp = x_max + y * z;      /* value in idx_t range       */
        seq_t val = (seq_t)val_tmp;         /* cast to seq_t for comparison */

        if (val < best) {
            best = val;
        }
    }

    return best;
}
