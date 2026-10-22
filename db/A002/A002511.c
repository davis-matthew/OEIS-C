/*
 * A002511  Expansion of a modular function for Gamma_0(21).
 * https://oeis.org/A002511
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x^6 * Product_{k>0} (1 - x^(21*k))^9 / ((1 - x^k) * (1 -
 *            x^(3*k))^3 * (1 - x^(7*k))^5), i.e. the Euler transform of the
 *            period-21 sequence [1,1,4,1,1,4,6,1,4,1,1,4,1,6,4,1,1,4,1,1,0]
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 74, u64 n <= 212,
 *            u128 n <= 688, big any n
 * Helpers:   a002511_mult, alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/a002511_mult.h"

static seq_t *a002511_c = NULL;
static idx_t a002511_len = 0;

static void a002511_build(idx_t len) {
    if (a002511_c) release(a002511_c, a002511_len);
    a002511_c = alloc_zero(len);
    a002511_len = len;
    a002511_c[0] = 1;
    for (idx_t k = 1; k < len; k++) {
        int m = a002511_mult(k);
        for (int r = 0; r < m; r++) {
            for (idx_t i = k; i < len; i++) {
                a002511_c[i] += a002511_c[i - k];
            }
        }
    }
}

seq_t a(idx_t n) {
    if (n < 6) return 0;
    idx_t need = n - 5;
    if (need > a002511_len) {
        idx_t len = a002511_len + a002511_len / 4;
        if (len < need) len = need;
        if (len < 64) len = 64;
        a002511_build(len);
    }
    return a002511_c[n - 6];
}
