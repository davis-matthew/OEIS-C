/*
 * A007793  Number of conjugacy classes of compact Cartan subgroups in
 *          Sp_{2n}(F), where p>n and the p-adic field F contains all r-th
 *          roots of unity for all r <= 2n.
 * https://oeis.org/A007793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{k>=1} 1/(1-x^k)^(6*sigma(k)), i.e. the Euler
 *            transform of 6*sigma(k). a(n) = (1/n) * Sum_{k=1..n}
 *            b(k)*a(n-k), a(0) = 1, where b(k) = Sum_{d|k} 6*d*sigma(d)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, seq_is_fixed_width, sigma_tab
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/sigma_tab.h"

#define FIXED_CAP 250

static uint64_t *bk = NULL;
static idx_t bcap = 0;

static void build_b(idx_t N) {
    uint64_t *sg = sigma_tab(N);
    free(bk);
    bk = (uint64_t *)calloc((size_t)N + 1, sizeof(uint64_t));
    for (idx_t d = 1; d <= N; d++) {
        uint64_t v = 6 * (uint64_t)d * sg[d];
        for (idx_t m = d; m <= N; m += d) bk[m] += v;
    }
    free(sg);
    bcap = N;
}

static seq_t *A = NULL;
static idx_t have = 0;

static seq_t fixed_vals[FIXED_CAP + 1];
static bool fixed_done = false;

static void build_fixed(void) {
    idx_t N = FIXED_CAP;
    uint64_t *sg = sigma_tab(N);
    for (idx_t m = 0; m <= N; m++) fixed_vals[m] = 0;
    fixed_vals[0] = 1;
    for (idx_t k = 1; k <= N; k++) {
        uint64_t c = 6 * sg[k];
        for (uint64_t r = 0; r < c; r++)
            for (idx_t m = k; m <= N; m++) fixed_vals[m] += fixed_vals[m - k];
    }
    free(sg);
    fixed_done = true;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        /* beyond FIXED_CAP the value exceeds 128 bits, so it cannot fit */
        if (n > FIXED_CAP) return 0;
        if (!fixed_done) build_fixed();
        return fixed_vals[n];
    }
    if (A == NULL) {
        A = alloc_zero(n + 1);
        A[0] = 1;
        have = 0;
    } else if (n > have) {
        A = grow_zero(A, have + 1, n + 1);
    }
    if (n > have) {
        if (bcap < n) build_b(n);
        for (idx_t m = have + 1; m <= n; m++) {
            seq_t s = 0;
            for (idx_t k = 1; k <= m; k++) s += (seq_t)bk[k] * A[m - k];
            A[m] = s / (seq_t)m;
        }
        have = n;
    }
    return A[n];
}
