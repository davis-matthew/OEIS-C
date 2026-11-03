/*
 * A005218  Number of unlabeled reduced unit interval graphs on n nodes.
 * https://oeis.org/A005218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: -z + (1/4)*(1+2z-z^2)/sqrt((1+z^2)*(1-3z^2)) -
 *            (1/4)*sqrt((1-3z)/(1+z)), i.e. for n >= 2: a(n) = (R(n-1) +
 *            T(k))/2 if n = 2k+1, a(n) = R(n-1)/2 + (T(k) - T(k-1))/4 if n =
 *            2k, where R = Riordan numbers A005043, T = central trinomial
 *            coefficients A002426
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 27, u64 n <= 48, u128 n <= 89,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 190 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

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

static void add2(seq_t *lo, seq_t *hi, seq_t xlo, seq_t xhi) {
    seq_t s = *lo + xlo;
    seq_t c = (s < *lo) ? 1 : 0;
    *hi = *hi + xhi + c;
    *lo = s;
}

/* Riordan number R(m) as a double-width (lo, hi) pair: Motzkin paths with no level step at height 0 */
static void riordan_add2(idx_t m, seq_t *rlo, seq_t *rhi) {
    idx_t L = m / 2 + 2, s, h;
    seq_t *lo = mem_new(seq_t, L);
    seq_t *hi = mem_new(seq_t, L);
    seq_t *nlo = mem_new(seq_t, L);
    seq_t *nhi = mem_new(seq_t, L);
    seq_t *tp;
    lo[0] = 1;
    for (s = 0; s < m; s++) {
        for (h = 0; h + 1 < L; h++) {
            seq_t vlo = lo[h + 1], vhi = hi[h + 1];
            if (h > 0) {
                add2(&vlo, &vhi, lo[h - 1], hi[h - 1]);
                add2(&vlo, &vhi, lo[h], hi[h]);
            }
            nlo[h] = vlo;
            nhi[h] = vhi;
        }
        nlo[L - 1] = 0;
        nhi[L - 1] = 0;
        tp = lo; lo = nlo; nlo = tp;
        tp = hi; hi = nhi; nhi = tp;
    }
    *rlo = lo[0];
    *rhi = hi[0];
    mem_free(lo); mem_free(hi); mem_free(nlo); mem_free(nhi);
}

static seq_t riordan_rec(idx_t m) {
    seq_t r0 = 1, r1 = 0, r2;
    idx_t i;
    if (m == 0) return r0;
    for (i = 2; i <= m; i++) {
        r2 = (seq_t)(i - 1) * ((seq_t)2 * r1 + (seq_t)3 * r0) / (seq_t)(i + 1);
        r0 = r1;
        r1 = r2;
    }
    return r1;
}

/* central trinomial coefficient by additions only (fixed-width builds) */
static seq_t trinom_central_add(idx_t k) {
    seq_t *row = mem_new(seq_t, k + 1);
    seq_t r;
    idx_t s, j, top;
    row[0] = 1;
    for (s = 1; s <= k; s++) {
        top = 2 * s < k ? 2 * s : k;
        for (j = top + 1; j-- > 0;) {
            seq_t v = row[j];
            if (j >= 1) v += row[j - 1];
            if (j >= 2) v += row[j - 2];
            row[j] = v;
        }
    }
    r = row[k];
    mem_free(row);
    return r;
}

/* central trinomial coefficient by recurrence (big-integer builds) */
static seq_t trinom_central_rec(idx_t k) {
    seq_t t0 = 1, t1 = 1, t2;
    idx_t i;
    if (k == 0) return t0;
    for (i = 2; i <= k; i++) {
        t2 = ((seq_t)(2 * i - 1) * t1 + (seq_t)(3 * (i - 1)) * t0) / (seq_t)i;
        t0 = t1;
        t1 = t2;
    }
    return t1;
}

seq_t a(idx_t n) {
    idx_t m, k;
    bool fixed;
    seq_t h;
    if (n <= 1) return 0;
    m = n - 1;
    k = n / 2;
    fixed = seq_is_fixed_width();
    if (n % 2 == 1) {
        h = fixed ? trinom_central_add(k) : trinom_central_rec(k);
    } else {
        seq_t tk = fixed ? trinom_central_add(k) : trinom_central_rec(k);
        seq_t tk1 = fixed ? trinom_central_add(k - 1) : trinom_central_rec(k - 1);
        h = (tk - tk1) / 2;
    }
    if (fixed) {
        seq_t lo, hi, x = 1;
        int w = 0;
        while (x != 0) { x <<= 1; w++; }
        riordan_add2(m, &lo, &hi);
        add2(&lo, &hi, h, 0);
        return (lo >> 1) | (hi << (w - 1));
    } else {
        seq_t r = riordan_rec(m);
        return (r + h) / 2;
    }
}
