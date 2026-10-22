/*
 * A006708  Expansion of a modular function for gamma_0(6).
 * https://oeis.org/A006708
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  sign, easy
 * Formula:   G.f. q^(-2) * Product_{k>=1} (1-q^k)^e(k) with e(k) = 1 + 7[2|k]
 *            + 5[3|k] - 13[6|k] (i.e. eta(z)*eta(2z)^7*eta(3z)^5/eta(6z)^13);
 *            computed via n*f(n) = -Sum_{m=1..n} s(m)*f(n-m), s(m) =
 *            Sum_{d|m} d*e(d), f(0)=1, a(n) = f(n+2); fixed-width builds
 *            expand the product mod 2^128
 * Range:     i8 n <= 4, i16 n <= 19, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   alloc_zero, ex_k, from_i128, grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/from_i128.h"
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/ex_k.h"

static unsigned __int128 *fw = NULL;
static idx_t fwlen = 0;

static seq_t *bf = NULL;
static idx_t bfcap = 0, bfcnt = 0;
static idx_t *sv = NULL;

seq_t a(idx_t n) {
    if (n < -2) return 0;
    idx_t m = n + 2;

    if (seq_is_fixed_width()) {
        if (m >= fwlen) {
            idx_t len = 2 * m + 1;
            if (len < 64) len = 64;
            unsigned __int128 *c = (unsigned __int128 *)malloc((size_t)len * sizeof(unsigned __int128));
            for (idx_t i = 0; i < len; i++) c[i] = 0;
            c[0] = 1;
            for (idx_t k = 1; k < len; k++) {
                int e = ex_k(k);
                if (e > 0) {
                    for (int p = 0; p < e; p++)
                        for (idx_t i = len - 1; i >= k; i--) c[i] -= c[i - k];
                } else if (e < 0) {
                    for (int p = 0; p < -e; p++)
                        for (idx_t i = k; i < len; i++) c[i] += c[i - k];
                }
            }
            free(fw);
            fw = c;
            fwlen = len;
        }
        return from_i128((__int128)fw[m]);
    }

    if (m >= bfcap) {
        idx_t cap = 2 * m + 1;
        if (cap < 64) cap = 64;
        if (bf == NULL) bf = alloc_zero(cap);
        else bf = grow_zero(bf, bfcap, cap);
        free(sv);
        sv = (idx_t *)malloc((size_t)cap * sizeof(idx_t));
        for (idx_t i = 0; i < cap; i++) sv[i] = 0;
        for (idx_t d = 1; d < cap; d++) {
            idx_t w = d * (idx_t)ex_k(d);
            if (w == 0) continue;
            for (idx_t j = d; j < cap; j += d) sv[j] += w;
        }
        bfcap = cap;
        if (bfcnt == 0) {
            bf[0] = 1;
            bfcnt = 1;
        }
    }
    while (bfcnt <= m) {
        idx_t j = bfcnt;
        seq_t acc = 0;
        for (idx_t k = 1; k <= j; k++) {
            if (sv[k] != 0) acc += bf[j - k] * (seq_t)sv[k];
        }
        bf[j] = -acc / j;
        bfcnt++;
    }
    return bf[m];
}
