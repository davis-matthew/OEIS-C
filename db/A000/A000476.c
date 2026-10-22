/*
 * A000476  Number of permutations of an n-sequence discordant with three
 *          given permutations (see reference) in n-1 places.
 * https://oeis.org/A000476
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k-1) * k * sigma_{n,k} * (n-k)!, i.e.
 *            the coefficient of y in Sum_{k=0..n} sigma_{n,k} (n-k)! (y-1)^k,
 *            where Sum sigma_{n,k} t^k u^n =
 *            (1-2t^2u^2-2t^2(1+t)u^3+3t^4u^4)/((1-tu)(1-(1+2t)u-tu^2+t^3u^3)).
 *            Rows S_m(t) = Sum_k sigma_{m,k} t^k satisfy S_m = (1+3t)S_{m-1}
 *            - 2t^2...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* signed coefficients kept as (pos, neg) pairs of seq_t */
static seq_t *SP[4], *SN[4];
static idx_t s_cap = 0;   /* each row array has s_cap entries */
static idx_t s_cur = 0;   /* rows 0..s_cur-1 computed (only last 4 kept) */

static void s_reset(void) {
    for (int r = 0; r < 4; r++)
        for (idx_t j = 0; j < s_cap; j++) { SP[r][j] = 0; SN[r][j] = 0; }
    s_cur = 0;
}

static void s_ensure(idx_t need) {
    if (need <= s_cap) return;
    idx_t nc = s_cap * 2;
    if (nc < need) nc = need;
    for (int r = 0; r < 4; r++) {
        seq_t *p = (seq_t *)malloc(nc * sizeof(seq_t));
        seq_t *q = (seq_t *)malloc(nc * sizeof(seq_t));
        for (idx_t j = 0; j < nc; j++) {
            if (j < s_cap) { p[j] = SP[r][j]; q[j] = SN[r][j]; }
            else { p[j] = 0; q[j] = 0; }
        }
        free(SP[r]); free(SN[r]);
        SP[r] = p; SN[r] = q;
    }
    s_cap = nc;
}

static void s_step(idx_t m) {
    int c0 = (int)(m & 3);
    int c1 = (int)((m + 3) & 3);
    int c2 = (int)((m + 2) & 3);
    int c3 = (int)((m + 1) & 3);
    int c4 = c0; /* row m-4 shares the slot of row m: read it before overwriting */
    for (idx_t jj = m + 1; jj-- > 0;) {
        idx_t j = jj;
        seq_t pos = 0, neg = 0;
        if (m >= 1) {
            pos += SP[c1][j]; neg += SN[c1][j];
            if (j >= 1) { pos += 3 * SP[c1][j - 1]; neg += 3 * SN[c1][j - 1]; }
        }
        if (m >= 2 && j >= 2) { pos += 2 * SN[c2][j - 2]; neg += 2 * SP[c2][j - 2]; }
        if (m >= 3) {
            if (j >= 2) { pos += SN[c3][j - 2]; neg += SP[c3][j - 2]; }
            if (j >= 3) { pos += SN[c3][j - 3]; neg += SP[c3][j - 3]; }
        }
        if (m >= 4 && j >= 4) { pos += SP[c4][j - 4]; neg += SN[c4][j - 4]; }
        if (m == 0 && j == 0) pos += 1;
        if (m == 2 && j == 2) neg += 2;
        if (m == 3 && (j == 2 || j == 3)) neg += 2;
        if (m == 4 && j == 4) pos += 3;
        if (pos >= neg) { pos -= neg; neg = 0; }
        else { neg -= pos; pos = 0; }
        /* descending j: entries j-4.. of row m-4 still unread are below j */
        SP[c0][j] = pos; SN[c0][j] = neg;
    }
}

seq_t a(idx_t n) {
    s_ensure(n + 5);
    if (s_cur > n + 1) s_reset();
    while (s_cur <= n) { s_step(s_cur); s_cur++; }

    int r = (int)(n & 3);
    seq_t pos = 0, neg = 0;
    seq_t fact = 1; /* (n-k)! */
    for (idx_t k = n; k >= 1; k--) {
        seq_t w = (seq_t)k * fact;
        if (k & 1) { pos += w * SP[r][k]; neg += w * SN[r][k]; }
        else { pos += w * SN[r][k]; neg += w * SP[r][k]; }
        fact *= (seq_t)(n - k + 1);
    }
    return pos - neg;
}
