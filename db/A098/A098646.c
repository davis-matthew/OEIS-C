/*
 * A098646  Trace sequence of 3 X 3 Krawtchouk matrix.
 * https://oeis.org/A098646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (-2)^n + 2*2^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: fast power of 4. */
static seq_t pow4(idx_t e) {
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)4;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Use the closed form derived from the recurrence
       a(0)=3, a(1)=2, a(n)=4*a(n-2). */
    if ((n & 1) == 0) {          /* n even: a(2k) = 3*4^k */
        idx_t k = n >> 1;
        return (seq_t)3 * pow4(k);
    } else {                     /* n odd: a(2k+1) = 2*4^k */
        idx_t k = n >> 1;
        return (seq_t)2 * pow4(k);
    }
}
