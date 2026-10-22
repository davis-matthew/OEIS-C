/*
 * A006150  Number of 4-tuples (p_1, p_2, ..., p_4) of Dyck paths of
 *          semilength n, such that each p_i is never below p_{i-1}.
 * https://oeis.org/A006150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*(2n-1)*(2n+1)*(2n+3)*(2n+5)*a(n-1) /
 *            ((n+4)*(n+5)*(n+6)*(n+7)), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   a006150_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006150_gcd.h"

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        uint64_t num[5];
        uint64_t den[4];
        int i, j;
        num[0] = 16;
        num[1] = 2 * (uint64_t)k - 1;
        num[2] = 2 * (uint64_t)k + 1;
        num[3] = 2 * (uint64_t)k + 3;
        num[4] = 2 * (uint64_t)k + 5;
        den[0] = (uint64_t)k + 4;
        den[1] = (uint64_t)k + 5;
        den[2] = (uint64_t)k + 6;
        den[3] = (uint64_t)k + 7;

        /* cancel denominators against the current value first */
        for (i = 0; i < 4; i++) {
            for (;;) {
                uint64_t r, g;
                if (den[i] == 1) break;
                r = (uint64_t)(v % (seq_t)den[i]);
                g = a006150_gcd(r, den[i]);
                if (g <= 1) break;
                v /= (seq_t)g;
                den[i] /= g;
            }
        }
        /* remaining denominators divide the numerator product */
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 5 && den[i] > 1; j++) {
                for (;;) {
                    uint64_t g = a006150_gcd(den[i], num[j]);
                    if (g <= 1) break;
                    den[i] /= g;
                    num[j] /= g;
                }
            }
        }
        for (j = 0; j < 5; j++) {
            if (num[j] != 1) v *= (seq_t)num[j];
        }
    }
    return v;
}
