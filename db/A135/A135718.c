/*
 * A135718  a(n) = smallest divisor of n^2 that is not a divisor of n.
 * https://oeis.org/A135718
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest divisor of n^2 that is not a divisor of n
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 9661, u64 n <= 9661,
 *            u128 n <= 9661, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;          /* offset starts at 2 */

    idx_t nn = n;
    seq_t best = 0;               /* 0 indicates not yet set */

    /* trial division to factor n */
    for (idx_t p = 2; p <= nn / p; ++p) {
        if (nn % p == 0) {
            idx_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }

            /* compute candidate = p^(e+1) */
            seq_t cand = 1;
            /* multiply e+1 times */
            for (idx_t i = 0; i <= e; ++i) cand *= (seq_t)p;

            if (best == 0 || cand < best) best = cand;
        }
    }

    /* remaining prime factor (if any) */
    if (nn > 1) {
        seq_t cand = (seq_t)nn * (seq_t)nn;   /* p^2 for the remaining prime */
        if (best == 0 || cand < best) best = cand;
    }

    return best;
}
