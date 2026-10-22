/*
 * A007780  Losing initial configurations in 2-hole Tchuka Ruma.
 * https://oeis.org/A007780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) + c(n), a(1) = 1, a(2) = 2, where c(n) are the
 *            coefficients x^n of
 *            x*(1+2x-7x^5-9x^6-13x^7-27x^8-30x^9-27x^10-9x^11-75x^13-243x^14-18x^15)
 *            (G.f.: that polynomial / (1 - 3*x^2)); so a(n) = 3*a(n-2) for n
 *            > 16
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 45, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int corr[17] = {
    0, 1, 2, 0, 0, 0, -7, -9, -13, -27, -30, -27, -9, 0, -75, -243, -18
};

seq_t a(idx_t n) {
    seq_t prev2 = 1, prev1 = 2; /* a(1), a(2) */
    idx_t k;
    if (n == 1) return prev2;
    if (n == 2) return prev1;
    for (k = 3; k <= n; k++) {
        seq_t cur = prev2 * 3;
        if (k <= 16) {
            int c = corr[k];
            if (c >= 0) cur += (seq_t)c;
            else cur -= (seq_t)(-c);
        }
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
