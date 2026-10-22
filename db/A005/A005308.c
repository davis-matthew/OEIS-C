/*
 * A005308  Bosonic string states.
 * https://oeis.org/A005308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n-1)] Product_{k>=1} (1 - x^k)^(-c(k)), c(k) =
 *            floor((k-2)/2) for k >= 2, c(1) = 0; arbitrary precision uses
 *            the Euler-transform recurrence m*b(m) = Sum_{k=1..m}
 *            s(k)*b(m-k), b(0) = 1, with s(k) = Sum_{d|k} d*c(d); fixed width
 *            multiplies the series by 1/(1-x^k) c(k) times (additions only)
 * Range:     u8 n <= 22, u16 n <= 43, u32 n <= 94, u64 n <= 220,
 *            u128 n <= 548, big any n
 * Helpers:   alloc_zero, c_of, grow_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"
#include "../utils/c_of.h"

/* fixed-width: series product with overflow tracking */
static seq_t *fb = NULL;
static bool *ff = NULL;
static idx_t fN = 0;
static bool fover = false;

static void fixed_build(idx_t N) {
    if (fb) release(fb, fN + 1);
    if (ff) free(ff);
    fb = alloc_zero(N + 1);
    ff = (bool *)calloc(N + 1, sizeof(bool));
    fN = N;
    fover = false;
    fb[0] = 1;
    for (idx_t k = 4; k <= N; k++) {
        idx_t c = c_of(k);
        for (idx_t r = 0; r < c; r++) {
            for (idx_t m = k; m <= N; m++) {
                seq_t v = fb[m] + fb[m - k];
                if (v < fb[m] || ff[m - k]) { ff[m] = true; fover = true; }
                fb[m] = v;
            }
        }
    }
}

/* arbitrary precision: Euler transform recurrence, memoized */
static seq_t *bb = NULL;
static uint64_t *sg = NULL;
static idx_t bN = 0;
static bool binit = false;

static void big_extend(idx_t N) {
    idx_t k, d;
    if (!binit) {
        bb = alloc_zero(1);
        bb[0] = 1;
        bN = 0;
        binit = true;
    }
    if (N <= bN) return;
    bb = grow_zero(bb, bN + 1, N + 1);
    if (sg) free(sg);
    sg = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    for (d = 4; d <= N; d++) {
        uint64_t w = (uint64_t)d * (uint64_t)c_of(d);
        for (k = d; k <= N; k += d) sg[k] += w;
    }
    for (idx_t m = bN + 1; m <= N; m++) {
        seq_t s = 0;
        for (k = 4; k <= m; k++) s += (seq_t)sg[k] * bb[m - k];
        bb[m] = s / (seq_t)m;
    }
    bN = N;
}

seq_t a(idx_t n) {
    idx_t m = n - 1;
    if (!seq_is_big()) {
        if (fb == NULL || (m > fN && !fover)) {
            idx_t N = fN ? fN : 64;
            while (N < m) N *= 2;
            fixed_build(N);
        }
        if (m > fN) return 0; /* value cannot fit in seq_t */
        return fb[m];
    }
    big_extend(m);
    return bb[m];
}
