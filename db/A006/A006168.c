/*
 * A006168  Number of factorization patterns of polynomials of degree n over
 *          F_3.
 * https://oeis.org/A006168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Euler transform of b(n) = sum_{d|n, A027376(d) >= n/d} 1,
 *            where A027376(d) = (1/d)*sum_{e|d} mu(e)*3^(d/e). Equivalently
 *            a(0) = 1, a(n) = (1/n)*sum_{k=1..n} c(k)*a(n-k), c(k) =
 *            sum_{d|k} d*b(d); fixed-width builds use the product prod_k
 *            1/(1-x^k)^b(k) with saturating adds
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 77, u64 n <= 231,
 *            u128 n <= 735, big any n
 * Helpers:   alloc_zero, grow_zero, release, sat_add, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/sat_add.h"

#define IRR_MAX 38

static int64_t irr[IRR_MAX + 1];
static bool irr_init = false;

static int mobius(idx_t n) {
    int cnt = 0;
    for (idx_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return 0;
            cnt++;
        }
    }
    if (n > 1) cnt++;
    return (cnt & 1) ? -1 : 1;
}

static void init_irr(void) {
    if (irr_init) return;
    for (idx_t d = 1; d <= IRR_MAX; d++) {
        int64_t s = 0;
        for (idx_t e = 1; e <= d; e++) {
            if (d % e) continue;
            int m = mobius(e);
            if (!m) continue;
            int64_t p = 1;
            for (idx_t i = 0; i < d / e; i++) p *= 3;
            s += m * p;
        }
        irr[d] = s / (int64_t)d;
    }
    irr_init = true;
}

/* does divisor d of k (with cofactor e = k/d) count: A027376(d) >= e */
static int ok(idx_t d, idx_t e) {
    if (d > IRR_MAX) return 1;
    return irr[d] >= (int64_t)e;
}

static int bval(idx_t k) {
    int cnt = 0;
    for (idx_t d = 1; d * d <= k; d++) {
        if (k % d) continue;
        idx_t e = k / d;
        if (ok(d, e)) cnt++;
        if (e != d && ok(e, d)) cnt++;
    }
    return cnt;
}

/* fixed-width state */
static seq_t *fw = NULL;
static idx_t fw_n = 0;

/* big state */
static seq_t *bg = NULL;
static seq_t *cc = NULL;
static idx_t bg_n = 0;

seq_t a(idx_t n) {
    init_irr();
    if (seq_is_fixed_width()) {
        seq_t mx = (seq_t)0;
        mx -= 1;
        if (fw != NULL && n <= fw_n) return fw[n];
        if (fw != NULL && fw[fw_n] == mx) return mx; /* nondecreasing, already saturated */
        idx_t N = n;
        if (N < 2 * fw_n) N = 2 * fw_n;
        if (N < 64) N = 64;
        if (fw != NULL) release(fw, fw_n + 1);
        fw = alloc_zero(N + 1);
        fw[0] = 1;
        for (idx_t k = 1; k <= N; k++) {
            int bk = bval(k);
            for (int t = 0; t < bk; t++)
                for (idx_t j = k; j <= N; j++)
                    fw[j] = sat_add(fw[j], fw[j - k]);
        }
        fw_n = N;
        return fw[n];
    }

    if (bg == NULL) {
        bg = alloc_zero(1);
        cc = alloc_zero(1);
        bg[0] = 1;
        bg_n = 0;
    }
    if (n <= bg_n) return bg[n];
    bg = grow_zero(bg, bg_n + 1, n + 1);
    cc = grow_zero(cc, bg_n + 1, n + 1);
    for (idx_t k = bg_n + 1; k <= n; k++) {
        seq_t s = 0;
        for (idx_t d = 1; d * d <= k; d++) {
            if (k % d) continue;
            idx_t e = k / d;
            s += (seq_t)d * bval(d);
            if (e != d) s += (seq_t)e * bval(e);
        }
        cc[k] = s;
    }
    for (idx_t i = bg_n + 1; i <= n; i++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= i; k++)
            s += cc[k] * bg[i - k];
        bg[i] = s / (seq_t)i;
    }
    bg_n = n;
    return bg[n];
}
