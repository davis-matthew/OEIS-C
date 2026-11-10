/*
 * A074681  Permutation of natural numbers induced by the Catalan bijection
 *          gmA074681! acting on the parenthesizations encoded by
 *          A014486/A063171.
 * https://oeis.org/A074681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of f(w_n) among Dyck words, where w_n is the n-th
 *            Dyck word (A014486 order) and f is the Catalan bijection with
 *            f(empty)=empty, f((A)) = () f(A), f((A)(S)T) = ( f((A)S) ) f(T)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define GM_R 100

static uint64_t gm_cnt[GM_R + 2][GM_R + 2];
static bool gm_init = false;

static uint64_t gm_sat_add(uint64_t x, uint64_t y)
{
    uint64_t s = x + y;
    return (s < x) ? UINT64_MAX : s;
}

static void gm_setup(void)
{
    int r, b;
    if (gm_init) return;
    for (r = 0; r <= GM_R + 1; ++r)
        for (b = 0; b <= GM_R + 1; ++b)
            gm_cnt[r][b] = 0;
    gm_cnt[0][0] = 1;
    for (r = 1; r <= GM_R; ++r) {
        for (b = 0; b <= r; ++b) {
            uint64_t v = 0;
            if (b > 0) v = gm_cnt[r - 1][b - 1];
            v = gm_sat_add(v, gm_cnt[r - 1][b + 1]);
            gm_cnt[r][b] = v;
        }
    }
    gm_init = true;
}

/* the bijection on parenthesis strings */
static void gm_f(const char *w, int len, char *out)
{
    int i, d, m = 0, p = 0;
    if (len == 0) return;
    d = 0;
    for (i = 0; i < len; ++i) {
        d += (w[i] == '(') ? 1 : -1;
        if (d == 0) { m = i; break; }
    }
    if (m == len - 1) {
        out[0] = '(';
        out[1] = ')';
        gm_f(w + 1, len - 2, out + 2);
        return;
    }
    {
        const char *B = w + m + 1;
        int blen = len - m - 1;
        int slen, tl, tl1;
        char tmp[GM_R * 2 + 8];
        d = 0;
        for (i = 0; i < blen; ++i) {
            d += (B[i] == '(') ? 1 : -1;
            if (d == 0) { p = i; break; }
        }
        slen = p - 1;
        tl = blen - p - 1;
        for (i = 0; i <= m; ++i) tmp[i] = w[i];
        for (i = 0; i < slen; ++i) tmp[m + 1 + i] = B[1 + i];
        tl1 = m + 1 + slen;
        out[0] = '(';
        gm_f(tmp, tl1, out + 1);
        out[1 + tl1] = ')';
        gm_f(B + p + 1, tl, out + 2 + tl1);
    }
}

seq_t a(idx_t n)
{
    char w[GM_R * 2 + 8], o[GM_R * 2 + 8];
    uint64_t idx = (uint64_t)n, cum = 0, cat;
    int k = 1, len, pos, bal;
    uint64_t rank;

    if (n == 0) return (seq_t)0;
    gm_setup();

    idx -= 1;
    for (;;) {
        cat = gm_cnt[2 * k][0];
        if (idx < cat) break;
        idx -= cat;
        cum += cat;
        ++k;
        if (k >= GM_R / 2) break;
    }
    len = 2 * k;

    /* unrank */
    bal = 0;
    for (pos = 0; pos < len; ++pos) {
        int rem = len - pos;
        uint64_t c0 = 0;
        if (bal > 0) c0 = gm_cnt[rem - 1][bal - 1];
        if (idx < c0) {
            w[pos] = ')';
            bal -= 1;
        } else {
            idx -= c0;
            w[pos] = '(';
            bal += 1;
        }
    }

    gm_f(w, len, o);

    /* rank */
    rank = 0;
    bal = 0;
    for (pos = 0; pos < len; ++pos) {
        int rem = len - pos;
        if (o[pos] == '(') {
            if (bal > 0) rank += gm_cnt[rem - 1][bal - 1];
            bal += 1;
        } else {
            bal -= 1;
        }
    }
    return (seq_t)(cum + rank + 1);
}
