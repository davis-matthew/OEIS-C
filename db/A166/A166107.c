/*
 * A166107  A sequence related to the Madhava-Gregory-Leibniz formula for Pi.
 * https://oeis.org/A166107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A166107: a(n) = p(n)*(-1)^n*4*sum_{k=1..n}(-1)^(k+1)/(2k-1)+2/(2n+1),
   where p(n)=∏_{i=floor((n-1)/3)+1}^{n}(2i+1).  Recurrence:
   a(0)=2, a(n)= -r*a(n-1) - 4*p(n)/((2n-1)(2n+1)),
   r = (2n+1)            if floor((n-1)/3) == floor((n-2)/3)
       (2n+1)/(2*prev_m+3) otherwise.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;

    seq_t a_prev = (seq_t)2;        /* a(0) */
    seq_t p = (seq_t)1;            /* p(0) = 1 */
    int m = -1;                    /* floor((n-1)/3) for current i, starts at -1 for n=0 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t r;
        int prev_m = m;               /* m for n-1 */

        if (i % 3 == 1) {             /* m increases by 1 */
            seq_t denom = (seq_t)(2 * prev_m + 3);
            r = ((seq_t)(2 * i + 1)) / denom;
            p = p * (seq_t)(2 * i + 1) / denom;
            ++m;
        } else {                       /* m stays the same */
            r = (seq_t)(2 * i + 1);
            p = p * (seq_t)(2 * i + 1);
            /* m unchanged */
        }

        seq_t denom_factor = (seq_t)(2 * i - 1) * (seq_t)(2 * i + 1);
        seq_t factor = (seq_t)4 * p / denom_factor;

        a_prev = -(r * a_prev) - factor;
    }

    return a_prev;
}
