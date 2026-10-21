/*
 * A235602  a(n) = n/wt(n) if wt(n) divides n, otherwise a(n) = n, where wt(n)
 *          is the binary weight of n (A000120).
 * https://oeis.org/A235602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n / wt(n) if wt(n) | n; otherwise a(n) = n, where wt(n)
 *            is the binary weight (number of 1 bits in n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Convert index to sequence type */
    seq_t nn = (seq_t)n;
    /* Compute binary weight of nn */
    unsigned wt = 0;
    seq_t tmp = nn;
    while (tmp) {
        wt += (unsigned)(tmp & 1);
        tmp >>= 1;
    }
    /* Avoid division by zero (won't happen for n >= 1) */
    if (wt == 0) return nn;
    /* Compute division and check divisibility */
    seq_t divisor = (seq_t)wt;
    seq_t div = nn / divisor;
    if (div * divisor == nn) {
        return div;   /* wt divides n */
    } else {
        return nn;    /* not divisible */
    }
}
