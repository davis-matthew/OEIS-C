/*
 * A006081  Number of line-rooted projective plane trees with n nodes.
 * https://oeis.org/A006081
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (C(n-1) + 2^(n-1) - binomial(n-1, floor((n-1)/2)) + [n
 *            even]*2*C(n/2-1)) / 4, where C(m) = binomial(2m,m)/(m+1) is the
 *            Catalan number (Burnside count of edge-rooted plane trees under
 *            end-swap and reflection)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 21, u64 n <= 38, u128 n <= 71,
 *            big any n
 * Helpers:   s_binom, s_cat, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/s_binom.h"
#include "../utils/s_cat.h"

#define WL 8
typedef struct { uint32_t d[WL]; } wnum;

static void w_set(wnum *x, uint32_t v)
{
    int i;
    for (i = 0; i < WL; i++) x->d[i] = 0;
    x->d[0] = v;
}

static void w_mul(wnum *x, uint32_t k)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < WL; i++) {
        uint64_t t = (uint64_t)x->d[i] * k + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void w_div(wnum *x, uint32_t k)
{
    uint64_t r = 0;
    int i;
    for (i = WL - 1; i >= 0; i--) {
        uint64_t t = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(t / k);
        r = t % k;
    }
}

static void w_add(wnum *x, const wnum *y)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < WL; i++) {
        uint64_t t = (uint64_t)x->d[i] + y->d[i] + c;
        x->d[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void w_sub(wnum *x, const wnum *y)
{
    int64_t b = 0;
    int i;
    for (i = 0; i < WL; i++) {
        int64_t t = (int64_t)x->d[i] - (int64_t)y->d[i] - b;
        if (t < 0) { t += ((int64_t)1 << 32); b = 1; } else b = 0;
        x->d[i] = (uint32_t)t;
    }
}

static void w_binom(wnum *x, uint32_t m, uint32_t k)
{
    uint32_t i;
    w_set(x, 1);
    for (i = 0; i < k; i++) {
        w_mul(x, m - i);
        w_div(x, i + 1);
    }
}

static void w_cat(wnum *x, uint32_t m)
{
    w_binom(x, 2 * m, m);
    w_div(x, m + 1);
}

seq_t a(idx_t n)
{
    if (seq_is_fixed_width() && n <= 120) {
        wnum t, u;
        seq_t r = 0;
        int i, j;
        uint32_t m = (uint32_t)n;
        w_cat(&t, m - 1);
        w_set(&u, 0);
        u.d[(m - 1) / 32] = (uint32_t)1 << ((m - 1) % 32);
        w_add(&t, &u);
        w_binom(&u, m - 1, (m - 1) / 2);
        w_sub(&t, &u);
        if (m % 2 == 0) {
            w_cat(&u, m / 2 - 1);
            w_mul(&u, 2);
            w_add(&t, &u);
        }
        w_div(&t, 4);
        for (i = WL - 1; i >= 0; i--)
            for (j = 31; j >= 0; j--) {
                r = (seq_t)(r + r);
                if ((t.d[i] >> j) & 1u) r = (seq_t)(r + 1);
            }
        return r;
    } else {
        seq_t r, p = 1;
        idx_t i;
        for (i = 0; i + 1 < n; i++) p *= 2;
        r = s_cat(n - 1) + p;
        r -= s_binom(n - 1, (n - 1) / 2);
        if (n % 2 == 0) r += 2 * s_cat(n / 2 - 1);
        return r / 4;
    }
}
