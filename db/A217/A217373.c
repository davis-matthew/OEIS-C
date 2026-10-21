/*
 * A217373  Numbers n the representation of which in base 3 ends in 2*k or
 *          2*k+1 zeros, where k is odd.
 * https://oeis.org/A217373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 * k, where k is the n-th positive integer with 3-adic
 *            valuation v3(k) ≡ 0 or 1 (mod 4)
 * Range:     u8 n <= 25, u16 n <= 6553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t k = 1;          /* candidate integer whose 3-valuation we test */
    idx_t count = 0;      /* how many qualifying integers seen so far */

    for (;;) {
        idx_t m = k;
        idx_t t = 0;               /* exponent of 3 in k */
        while (m % 3 == 0) {
            t++;
            m /= 3;
        }
        if ((t % 4) <= 1) {        /* v3(k) ≡ 0,1 (mod 4) → acceptable */
            count++;
            if (count == n) {
                return (seq_t)9 * k;   /* a(n) = 9 * k */
            }
        }
        ++k;
    }
    /* unreachable, but keeps the compiler happy */
    return 0;
}
