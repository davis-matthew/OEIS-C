/*
 * A001867  Number of n-bead necklaces with 3 colors.
 * https://oeis.org/A001867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/n)*Sum_{d|n} phi(d)*3^(n/d), n>0; a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   a1867_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1867_phi.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    /* Accumulate sum/n as Q + R/n, keeping each 3^e as q*n + r so that
       the quotient stays exact (mod 2^w in fixed-width builds). */
    seq_t Q = 0;
    uint64_t R = 0;
    idx_t d;
    for (d = 1; d <= n; d++) {
        if (n % d != 0) continue;
        idx_t e = n / d, i;
        /* 1 = q*n + r */
        seq_t q = (seq_t)(1 / n);
        uint64_t r = 1 % n;
        for (i = 0; i < e; i++) {
            uint64_t t = r * 3;
            q = q * 3 + (seq_t)(t / n);
            r = t % n;
        }
        uint64_t ph = a1867_phi(d);
        Q += q * (seq_t)ph;
        R += ph * r;
        Q += (seq_t)(R / n);
        R %= n;
    }
    return Q;
}
