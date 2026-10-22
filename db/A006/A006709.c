/*
 * A006709  Expansion of a modular function.
 * https://oeis.org/A006709
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  sign, easy
 * Formula:   G.f.: x^-2 * Product_{k>0} (1-x^k)^2 * (1-x^(2k))^2 *
 *            (1-x^(3k))^10 / (1-x^(6k))^14 = x^-2 * Product_{m>0}
 *            (1-x^m)^c(m), with c(m) = 2,4,12,4,2,0 for m mod 6 =
 *            1,2,3,4,5,0. Fixed-width builds multiply the factors out exactly
 *            modulo 2^128 (only ring operations, so every term that fits
 *            comes out ri...
 * Range:     i8 n <= 5, i16 n <= 19, i32 n <= 73, i64 n <= 265,
 *            i128 n <= 1000, big any n
 * Helpers:   alloc_zero, cexp6, grow_zero, release, seq_is_fixed_width,
 *            wrap_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/cexp6.h"
#include "../utils/wrap_to_seq.h"

/* ---- fixed-width path: exact arithmetic modulo 2^128 ---- */
static unsigned __int128 *fw = NULL;
static idx_t fw_len = 0;

static void fw_ensure(idx_t need) {
    if (need <= fw_len) return;
    idx_t L = fw_len * 2;
    if (L < need) L = need;
    if (L < 64) L = 64;
    free(fw);
    fw = (unsigned __int128 *)calloc((size_t)L, sizeof(unsigned __int128));
    fw[0] = 1;
    for (idx_t m = 1; m < L; m++) {
        int c = cexp6(m);
        for (int t = 0; t < c; t++)
            for (idx_t i = L - 1; i >= m; i--)
                fw[i] -= fw[i - m];
    }
    fw_len = L;
}

/* ---- arbitrary-precision path: Euler transform recurrence ---- */
static seq_t *bg = NULL;
static seq_t *bB = NULL;
static idx_t bg_len = 0;

static void bg_ensure(idx_t need) {
    if (need <= bg_len) return;
    idx_t L = bg_len * 2;
    if (L < need) L = need;
    if (L < 64) L = 64;
    if (bB) release(bB, bg_len);
    bB = alloc_zero(L);
    for (idx_t d = 1; d < L; d++) {
        int c = cexp6(d);
        if (c == 0) continue;
        for (idx_t k = d; k < L; k += d)
            bB[k] -= (seq_t)d * c;
    }
    bg = (bg == NULL) ? alloc_zero(L) : grow_zero(bg, bg_len, L);
    idx_t start = bg_len;
    if (start == 0) { bg[0] = 1; start = 1; }
    for (idx_t m = start; m < L; m++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= m; k++)
            s += bB[k] * bg[m - k];
        bg[m] = s / (seq_t)m;
    }
    bg_len = L;
}

seq_t a(idx_t n) {
    idx_t idx = n + 2;
    if (idx < 0) return 0;
    if (seq_is_fixed_width()) {
        fw_ensure(idx + 1);
        return wrap_to_seq(fw[idx]);
    }
    bg_ensure(idx + 1);
    return bg[idx];
}
