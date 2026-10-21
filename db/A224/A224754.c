/*
 * A224754  a(2)=3; thereafter a(n) = smallest number m such that a(n-1)+m =
 *          (a(n-1) followed by the leading digit of m).
 * https://oeis.org/A224754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the definition of OEIS sequence A224754:
   a(1) = 3
   for n > 1, a(n) is the smallest positive integer m such that
   a(n-1) + m = (a(n-1) followed by the first decimal digit of m).
   That is, m = 9·a(n-1) + d, where d is the first decimal digit of m.
   The smallest d (1…9) that satisfies this condition gives a(n). */
#include <stdint.h>
#include "../utils/pow10.h"   /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h" /* int digit_len(seq_t x); */

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1 */

    seq_t cur = (seq_t)3;          /* a(1) = 3 */
    if (n == 1) return cur;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = 0;
        /* test d from 1 to 9 and pick the first that yields a leading
           digit of d for m = 9*cur + d.  Because m increases with d,
           the first satisfying d gives the smallest m. */
        for (int d = 1; d <= 9; ++d) {
            seq_t m = cur * (seq_t)9 + (seq_t)d;

            /* Determine the leading digit of m. */
            int len = digit_len(m);
            seq_t base = pow10((idx_t)(len - 1));
            seq_t leading = m / base;

            if ((int)leading == d) {
                next = m;
                break;
            }
        }
        cur = next;
    }
    return cur;
}
