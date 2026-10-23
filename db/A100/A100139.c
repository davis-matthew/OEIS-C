/*
 * A100139  a(n) = Sum_{k=0..floor(n/6)} C(n-3k,3k) * 3^k * 2^(n-6k).
 * https://oeis.org/A100139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A100139
   a(n) = Sum_{k=0..⌊n/6⌋} C(n-3k,3k) * 3^k * 2^(n-6k)
   Recurrence: a(n) = 6*a(n-1) - 12*a(n-2) + 8*a(n-3) + 3*a(n-6)
   Base values: a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=32
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base values for n = 0..5 */
    static const seq_t base[6] = { (seq_t)1, (seq_t)2, (seq_t)4,
                                   (seq_t)8, (seq_t)16, (seq_t)32 };

    if (n < 6) {
        return base[n];
    }

    /* sliding window of the last six terms:
       last[0] = a(i-1), last[1] = a(i-2), ... last[5] = a(i-6)   */
    seq_t last[6];
    last[0] = base[5];
    last[1] = base[4];
    last[2] = base[3];
    last[3] = base[2];
    last[4] = base[1];
    last[5] = base[0];

    /* iterate from i = 6 up to n inclusive */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t term = 6 * last[0];
        term += 8 * last[2];
        term += 3 * last[5];
        term -= 12 * last[1];

        /* shift window: make space for the new term */
        for (int j = 5; j > 0; --j) {
            last[j] = last[j-1];
        }
        last[0] = term;
    }

    return last[0];
}
