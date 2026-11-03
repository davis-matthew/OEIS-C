/*
 * A007748  Number of self-converse oriented trees with n nodes.
 * https://oeis.org/A007748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(2n) = A000151(n), a(2n+1) = A005750(n+1), where A(x) = Sum
 *            A000151(k) x^k = x*E(x)^2 and E(x) = Sum e(m) x^m = Prod_{k>=1}
 *            (1-x^k)^(-A000151(k)). So A000151(k) = Sum_{i=0..k-1}
 *            e(i)*e(k-1-i), e(0) = 1, and m*e(m) = Sum_{d=1..m} d*A000151(d)
 *            * Sum_{j>=1} e(m-j*d); a(2n+1) = e(n)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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

static void mulwide(seq_t x, seq_t y, seq_t *hi, seq_t *lo)
{
    int W = (int)(8 * sizeof(seq_t)), H = W / 2;
    seq_t mask = ((seq_t)1 << H) - 1;
    seq_t x0 = x & mask, x1 = x >> H, y0 = y & mask, y1 = y >> H;
    seq_t p00 = x0 * y0, p01 = x0 * y1, p10 = x1 * y0, p11 = x1 * y1;
    seq_t mid = (p00 >> H) + (p01 & mask) + (p10 & mask);
    *lo = (p00 & mask) | (mid << H);
    *hi = p11 + (p01 >> H) + (p10 >> H) + (mid >> H);
}

static seq_t *E = 0, *R = 0, *C = 0;
static idx_t have = 0, cap = 0;

static void ensure(idx_t k)
{
    idx_t m;
    bool fixed;
    if (k < have) return;
    if (k + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < k + 1) nc = k + 1;
        E = grow_zero(E, cap, nc);
        R = grow_zero(R, cap, nc);
        C = grow_zero(C, cap, nc);
        cap = nc;
    }
    fixed = seq_is_fixed_width();
    for (m = have; m <= k; m++) {
        idx_t i, d;
        if (m == 0) {
            E[0] = 1; R[0] = 0; C[0] = 0;
            continue;
        }
        {
            seq_t s = 0;
            for (i = 0; i < m; i++) s += E[i] * E[m - 1 - i];
            R[m] = s;
        }
        if (!fixed) {
            seq_t c = 0, S = 0;
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t q = m / d;
                    c += (seq_t)d * R[d];
                    if (q != d) c += (seq_t)q * R[q];
                }
            }
            C[m] = c;
            for (i = 1; i <= m; i++) S += C[i] * E[m - i];
            E[m] = S / (seq_t)m;
        } else {
            seq_t shi = 0, slo = 0, q = 0;
            uint64_t rem = 0;
            int W = (int)(8 * sizeof(seq_t)), b;
            for (d = 1; d <= m; d++) {
                seq_t T = 0, h, l, h2, l2;
                idx_t j = m - d;
                for (;;) {
                    T += E[j];
                    if (j < d) break;
                    j -= d;
                }
                mulwide(R[d], T, &h, &l);
                mulwide(l, (seq_t)d, &h2, &l2);
                h = h * (seq_t)d + h2;
                l = l2;
                slo += l;
                if (slo < l) shi += 1;
                shi += h;
            }
            for (b = W - 1; b >= 0; b--) {
                rem = rem * 2 + (uint64_t)((shi >> b) & 1);
                if (rem >= (uint64_t)m) rem -= (uint64_t)m;
            }
            for (b = W - 1; b >= 0; b--) {
                rem = rem * 2 + (uint64_t)((slo >> b) & 1);
                if (rem >= (uint64_t)m) {
                    rem -= (uint64_t)m;
                    q |= (seq_t)1 << b;
                }
            }
            E[m] = q;
        }
    }
    have = k + 1;
}

seq_t a(idx_t n)
{
    idx_t k = n / 2;
    ensure(k);
    if (n % 2 == 0) return R[k];
    return E[k];
}
