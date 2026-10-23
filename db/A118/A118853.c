/*
 * A118853  Start with 1 and repeatedly place the first digit at the end of
 *          the number and add 10.
 * https://oeis.org/A118853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=1; for n>=2: a(n)=f(a(n-1))+10 where f(x) moves the first
 *            decimal digit of x to the end
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but safe */
    if (n == 1) return 1;
    if (n == 2) return 11;

    seq_t cur = 11;
    for (idx_t i = 3; i <= n; ++i) {
        /* find 10^(len(cur)-1) */
        seq_t pow10 = 1;
        while (pow10 <= cur / 10) {
            pow10 *= 10;
        }
        seq_t d   = cur / pow10;      /* leading digit */
        seq_t rest = cur % pow10;     /* remaining digits */
        cur = rest * 10 + d + 10;     /* move digit and add 10 */
    }
    return cur;
}
