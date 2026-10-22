/*
 * A002704  Number of sets with a congruence property.
 * https://oeis.org/A002704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   with q = 2n+1 and q' = q with all factors 7 removed, a(n) = (
 *            Sum_{d|q, 7 does not divide d} phi(d)*8^(q/d) + 7*q - q' ) /
 *            (7*q)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   a002704_is_fixed, a002704_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002704_is_fixed.h"
#include "../utils/a002704_phi.h"

seq_t a(idx_t n)
{
    uint64_t q = 2 * (uint64_t)n + 1;
    uint64_t qp = q, i;
    while (qp % 7 == 0) qp /= 7;

    if (a002704_is_fixed()) {
        /* exact arithmetic modulo 2^128; 7q is odd so it is invertible */
        typedef unsigned __int128 u128;
        u128 S = 0, m, inv, res;
        seq_t r = 0;
        int b;
        for (i = 1; i * i <= q; i++) {
            if (q % i) continue;
            uint64_t ds[2];
            int k, cnt = 1;
            ds[0] = i;
            if (i != q / i) { ds[1] = q / i; cnt = 2; }
            for (k = 0; k < cnt; k++) {
                uint64_t d = ds[k];
                if (d % 7 == 0) continue;
                uint64_t s = 3 * (q / d);
                if (s >= 128) continue;
                S += (u128)a002704_phi(d) * ((u128)1 << s);
            }
        }
        S += (u128)7 * q - qp;
        m = (u128)7 * q;
        inv = m;
        for (b = 0; b < 7; b++) inv = inv * ((u128)2 - m * inv);
        res = S * inv;
        for (b = 127; b >= 0; b--) {
            int bit = (int)((res >> b) & 1);
            r = r + r + bit;
        }
        return r;
    } else {
        seq_t S = 0;
        for (i = 1; i * i <= q; i++) {
            if (q % i) continue;
            uint64_t ds[2];
            int k, cnt = 1;
            ds[0] = i;
            if (i != q / i) { ds[1] = q / i; cnt = 2; }
            for (k = 0; k < cnt; k++) {
                uint64_t d = ds[k];
                if (d % 7 == 0) continue;
                idx_t s = (idx_t)(3 * (q / d));
                S += (seq_t)a002704_phi(d) * ((seq_t)1 << s);
            }
        }
        S += (seq_t)7 * (seq_t)q;
        S -= (seq_t)qp;
        return S / ((seq_t)7 * (seq_t)q);
    }
}
