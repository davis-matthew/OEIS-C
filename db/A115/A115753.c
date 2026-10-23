/*
 * A115753  If a(n) is a k-digit number, a(n+1) is the product of the number
 *          formed by the initial k-1 digits of a(n) and the final digit of
 *          a(n). If k=1, set a(n+1) = 0.
 * https://oeis.org/A115753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 102, u32 n <= 102, u64 n <= 102, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 11419, a(n+1) = 
   (a(n)/10) * (a(n)%10) if a(n) ≥ 10, else a(n+1)=0 */
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* safety, n≥1 by definition */
    seq_t cur = 11419;               /* a(1) */
    if (n == 1) return cur;
    for (idx_t i = 1; i < n; ++i) {
        if (cur < 10) {              /* digit count k=1 */
            cur = 0;
            break;
        }
        seq_t last = cur % (seq_t)10;
        cur = (cur / (seq_t)10) * last;
        if (cur == 0) break;
    }
    return cur;
}
