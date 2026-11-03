/*
 * A006202  Number of colorings of labeled graphs on n nodes using exactly 4
 *          colors, divided by 4!*2^6.
 * https://oeis.org/A006202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Sum_{a1+a2+a3+a4=n, ai>=1} multinomial(n; a1,a2,a3,a4) *
 *            2^(Sum_{i<j} ai*aj)) / (4!*2^6), computed via f_k(m) =
 *            Sum_{a=1..m-1} binomial(m,a) * 2^(a*(m-a)) * f_{k-1}(m-a),
 *            f_1(m) = 1 (m>=1); g3(m) = f_3(m)/48, a(n) = (Sum_{a=1..n-1}
 *            binomial(n,a) * 2^(a*(n-a)) * g3(n-a)) / 32
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 78 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

typedef unsigned __int128 u128;

static u128 sh128(idx_t e)
{
    if (e >= 128) return 0;
    return ((u128)1) << e;
}

static seq_t fixed_path(idx_t n)
{
    u128 F2[48], G3[48];
    idx_t m, a;
    u128 c, s;
    if (n > 40) return (seq_t)0;
    for (m = 0; m <= n; m++) {
        F2[m] = 0;
        G3[m] = 0;
        if (m < 2) continue;
        c = 1;
        s = 0;
        for (a = 1; a < m; a++) {
            c = c * (u128)(m - a + 1) / (u128)a;
            s += c * sh128(a * (m - a));
        }
        F2[m] = s;
        c = 1;
        s = 0;
        for (a = 1; a < m; a++) {
            c = c * (u128)(m - a + 1) / (u128)a;
            s += c * sh128(a * (m - a)) * F2[m - a];
        }
        G3[m] = s / 48;
    }
    c = 1;
    s = 0;
    for (a = 1; a < n; a++) {
        c = c * (u128)(n - a + 1) / (u128)a;
        s += c * sh128(a * (n - a)) * G3[n - a];
    }
    return u128_to_seq(s / 32);
}

static seq_t *F2b = NULL, *G3b = NULL;
static idx_t capb = 0;

static void ensure_big(idx_t n)
{
    idx_t m, a, newcap;
    seq_t c, s;
    if (n < capb) return;
    newcap = n + 1;
    F2b = grow_zero(F2b, capb, newcap);
    G3b = grow_zero(G3b, capb, newcap);
    for (m = capb; m < newcap; m++) {
        F2b[m] = 0;
        G3b[m] = 0;
        if (m < 2) continue;
        c = 1;
        s = 0;
        for (a = 1; a < m; a++) {
            c = c * (seq_t)(m - a + 1) / (seq_t)a;
            s += c * pow2(a * (m - a));
        }
        F2b[m] = s;
        c = 1;
        s = 0;
        for (a = 1; a < m; a++) {
            c = c * (seq_t)(m - a + 1) / (seq_t)a;
            s += c * pow2(a * (m - a)) * F2b[m - a];
        }
        G3b[m] = s / 48;
    }
    capb = newcap;
}

seq_t a(idx_t n)
{
    idx_t k;
    seq_t c, s;
    if (n < 4) return (seq_t)0;
    if (seq_is_fixed_width()) return fixed_path(n);
    ensure_big(n);
    c = 1;
    s = 0;
    for (k = 1; k < n; k++) {
        c = c * (seq_t)(n - k + 1) / (seq_t)k;
        s += c * pow2(k * (n - k)) * G3b[n - k];
    }
    return s / 32;
}
