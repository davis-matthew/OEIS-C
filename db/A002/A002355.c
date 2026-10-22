/*
 * A002355  Denominators of convergents to cube root of 4.
 * https://oeis.org/A002355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 0, a(0) = 1, where c(n) are
 *            the continued-fraction partial quotients of 4^(1/3), computed
 *            exactly with integers by Lagrange's method: start with P(x) =
 *            x^3 - 4; c = largest integer with P(c) < 0; replace P(x) by -x^3
 *            * P(c + 1/x); repeat
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 39, u128 n <= 82,
 *            big any n
 * Helpers:   a2355_add, a2355_isneg, a2355_mulsm, a2355_negate, a2355_roomy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2355_add.h"
#include "../utils/a2355_isneg.h"
#include "../utils/a2355_mulsm.h"
#include "../utils/a2355_negate.h"
#include "../utils/a2355_roomy.h"

static uint32_t *a2355_A = NULL, *a2355_B = NULL, *a2355_C = NULL, *a2355_D = NULL;
static uint32_t *a2355_t = NULL, *a2355_u = NULL, *a2355_w = NULL;
static size_t a2355_L = 0;
static uint32_t *a2355_cf = NULL;
static idx_t a2355_ncf = 0, a2355_cfcap = 0;

/* t = P(k); returns 1 if P(k) < 0 */
static int a2355_evalneg(uint32_t k) {
    size_t L = a2355_L;
    memcpy(a2355_t, a2355_A, L * sizeof(uint32_t));
    a2355_mulsm(a2355_t, L, k);
    a2355_add(a2355_t, a2355_B, L);
    a2355_mulsm(a2355_t, L, k);
    a2355_add(a2355_t, a2355_C, L);
    a2355_mulsm(a2355_t, L, k);
    a2355_add(a2355_t, a2355_D, L);
    return a2355_isneg(a2355_t, L);
}

static void a2355_grow(size_t nl) {
    uint32_t **cs[4];
    size_t i, j, L = a2355_L;
    cs[0] = &a2355_A; cs[1] = &a2355_B; cs[2] = &a2355_C; cs[3] = &a2355_D;
    for (i = 0; i < 4; i++) {
        uint32_t *p = (uint32_t *)realloc(*cs[i], nl * sizeof(uint32_t));
        uint32_t ext = (p[L - 1] >> 31) ? 0xFFFFFFFFu : 0u;
        for (j = L; j < nl; j++) p[j] = ext;
        *cs[i] = p;
    }
    a2355_t = (uint32_t *)realloc(a2355_t, nl * sizeof(uint32_t));
    a2355_u = (uint32_t *)realloc(a2355_u, nl * sizeof(uint32_t));
    a2355_w = (uint32_t *)realloc(a2355_w, nl * sizeof(uint32_t));
    a2355_L = nl;
}

static void a2355_init(void) {
    size_t L = 8, j;
    a2355_A = (uint32_t *)calloc(L, sizeof(uint32_t));
    a2355_B = (uint32_t *)calloc(L, sizeof(uint32_t));
    a2355_C = (uint32_t *)calloc(L, sizeof(uint32_t));
    a2355_D = (uint32_t *)malloc(L * sizeof(uint32_t));
    a2355_t = (uint32_t *)malloc(L * sizeof(uint32_t));
    a2355_u = (uint32_t *)malloc(L * sizeof(uint32_t));
    a2355_w = (uint32_t *)malloc(L * sizeof(uint32_t));
    a2355_A[0] = 1;
    for (j = 0; j < L; j++) a2355_D[j] = 0xFFFFFFFFu;
    a2355_D[0] = 0xFFFFFFFCu; /* -4 */
    a2355_L = L;
}

static uint32_t a2355_step(void) {
    size_t L;
    uint32_t k, c, *tmp;
    if (!a2355_roomy(a2355_A, a2355_L) || !a2355_roomy(a2355_B, a2355_L) ||
        !a2355_roomy(a2355_C, a2355_L) || !a2355_roomy(a2355_D, a2355_L))
        a2355_grow(a2355_L + a2355_L / 2 + 8);
    L = a2355_L;

    /* P(1) < 0 always (the single real root exceeds 1) */
    for (k = 2; k <= 16; k++)
        if (!a2355_evalneg(k)) break;
    if (k <= 16) {
        c = k - 1;
    } else {
        uint32_t lo = 16, hi = 32;
        while (hi < (1u << 30) && a2355_evalneg(hi)) { lo = hi; hi *= 2; }
        while (hi - lo > 1) {
            uint32_t mid = lo + (hi - lo) / 2;
            if (a2355_evalneg(mid)) lo = mid; else hi = mid;
        }
        c = lo;
    }

    a2355_evalneg(c); /* t = D' = P(c) */
    /* u = 3cA */
    memcpy(a2355_u, a2355_A, L * sizeof(uint32_t));
    a2355_mulsm(a2355_u, L, 3u * c);
    /* C' = C + c*(3cA + 2B) */
    memcpy(a2355_w, a2355_u, L * sizeof(uint32_t));
    a2355_add(a2355_w, a2355_B, L);
    a2355_add(a2355_w, a2355_B, L);
    a2355_mulsm(a2355_w, L, c);
    a2355_add(a2355_C, a2355_w, L);
    /* B' = B + 3cA */
    a2355_add(a2355_B, a2355_u, L);

    /* new poly: -D' x^3 - C' x^2 - B' x - A */
    a2355_negate(a2355_t, L);
    a2355_negate(a2355_C, L);
    a2355_negate(a2355_B, L);
    a2355_negate(a2355_A, L);
    tmp = a2355_D;
    a2355_D = a2355_A;
    a2355_A = a2355_t;
    a2355_t = tmp;
    tmp = a2355_B;
    a2355_B = a2355_C;
    a2355_C = tmp;
    return c;
}

seq_t a(idx_t n) {
    seq_t q0, q1, q2;
    idx_t i;
    if (a2355_L == 0) a2355_init();
    while (a2355_ncf <= n) {
        if (a2355_ncf >= a2355_cfcap) {
            a2355_cfcap = a2355_cfcap * 2 + 64;
            a2355_cf = (uint32_t *)realloc(a2355_cf, a2355_cfcap * sizeof(uint32_t));
        }
        a2355_cf[a2355_ncf] = a2355_step();
        a2355_ncf++;
    }
    q0 = 0;
    q1 = 1;
    for (i = 1; i <= n; i++) {
        q2 = (seq_t)a2355_cf[i] * q1 + q0;
        q0 = q1;
        q1 = q2;
    }
    return q1;
}
