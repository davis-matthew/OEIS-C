/*
 * A037844  Sum{d(i-1)-d(i): d(i)<d(i-1), i=1,...,m}, where Sum{d(i)*3^i:
 *          i=0,1,...,m} is base 3 representation of n.
 * https://oeis.org/A037844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Sum of local decreases in the base‑3 representation of n. a(n) =
 *            Σ_{i=1..m} (d_{i-1} - d_i) over all i where d_i < d_{i-1}, where
 *            d_0 .. d_m are the base‑3 digits of n (least to most
 *            significant)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint8_t, if needed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* not used in OEIS but harmless */

    seq_t sum = 0;
    idx_t t = n;

    /* first (least significant) digit */
    unsigned prev = (unsigned)(t % 3);
    t /= 3;

    while (t > 0) {
        unsigned cur = (unsigned)(t % 3);
        if (prev > cur) {
            sum += (seq_t)(prev - cur);
        }
        prev = cur;
        t /= 3;
    }

    return sum;
}
