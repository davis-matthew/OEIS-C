/*
 * A005446  Denominators of expansion of -W_{-1}(-e^{-1-x^2/2}) where W_{-1}
 *          is Lambert W function.
 * https://oeis.org/A005446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of ((-1)^n * b(n)), where b(n) = (1/(n+1))*(
 *            b(n-1) - Sum_{j=2..n-1} j*b(j)*b(n-j+1) ), b(0) = b(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
typedef std::conditional<std::is_class<seq_t>::value, seq_t, unsigned __int128>::type wide_t;
#else
typedef unsigned __int128 wide_t;
#endif

static wide_t *bnum = NULL, *bden = NULL;
static int *bsgn = NULL;
static idx_t cap = 0, have = 0;

static wide_t w_gcd(wide_t x, wide_t y) {
    while (y != 0) {
        wide_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static wide_t w_gcd1(wide_t x, wide_t y) {
    wide_t g = w_gcd(x, y);
    if (g == 0) g = 1;
    return g;
}

static void fr_reduce(wide_t *num, wide_t *den, int *sg) {
    if (*num == 0) {
        *den = 1;
        *sg = 1;
        return;
    }
    wide_t g = w_gcd1(*num, *den);
    if (g > 1) {
        *num /= g;
        *den /= g;
    }
}

static void fr_add(wide_t an, int as, wide_t ad, wide_t bn, int bs, wide_t bd,
                   wide_t *rn, int *rs, wide_t *rd) {
    if (an == 0) { *rn = bn; *rs = bs; *rd = bd; return; }
    if (bn == 0) { *rn = an; *rs = as; *rd = ad; return; }
    wide_t g = w_gcd1(ad, bd);
    wide_t x = an * (bd / g);
    wide_t y = bn * (ad / g);
    wide_t den = (ad / g) * bd;
    if (as == bs) {
        *rn = x + y;
        *rs = as;
    } else if (x >= y) {
        *rn = x - y;
        *rs = as;
    } else {
        *rn = y - x;
        *rs = bs;
    }
    *rd = den;
    fr_reduce(rn, rd, rs);
}

static void fr_mul(wide_t an, int as, wide_t ad, wide_t bn, int bs, wide_t bd,
                   wide_t *rn, int *rs, wide_t *rd) {
    if (an == 0 || bn == 0) { *rn = 0; *rs = 1; *rd = 1; return; }
    wide_t g1 = w_gcd1(an, bd);
    wide_t g2 = w_gcd1(bn, ad);
    *rn = (an / g1) * (bn / g2);
    *rd = (ad / g2) * (bd / g1);
    *rs = as * bs;
}

static void ensure(idx_t n) {
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < n + 1) nc *= 2;
        wide_t *nn = (wide_t *)calloc((size_t)nc, sizeof(wide_t));
        wide_t *nd = (wide_t *)calloc((size_t)nc, sizeof(wide_t));
        for (idx_t i = 0; i < have; i++) {
            nn[i] = bnum[i];
            nd[i] = bden[i];
        }
        free(bnum);
        free(bden);
        bnum = nn;
        bden = nd;
        bsgn = (int *)realloc(bsgn, (size_t)nc * sizeof(int));
        cap = nc;
    }
    if (have == 0) {
        bnum[0] = 1; bden[0] = 1; bsgn[0] = 1;
        bnum[1] = 1; bden[1] = 1; bsgn[1] = 1;
        have = 2;
    }
    for (idx_t i = have; i <= n; i++) {
        wide_t sn = 0, sd = 1;
        int ss = 1;
        for (idx_t j = 2; j < i; j++) {
            wide_t tn = bnum[j], td = bden[j];
            int ts = bsgn[j];
            if (tn != 0) {
                wide_t jj = (wide_t)j;
                wide_t g = w_gcd1(jj, td);
                tn *= (jj / g);
                td /= g;
            }
            wide_t pn, pd;
            int ps;
            fr_mul(tn, ts, td, bnum[i - j + 1], bsgn[i - j + 1], bden[i - j + 1],
                   &pn, &ps, &pd);
            wide_t rn, rd;
            int rs;
            fr_add(sn, ss, sd, pn, ps, pd, &rn, &rs, &rd);
            sn = rn; ss = rs; sd = rd;
        }
        wide_t dn, dd;
        int ds;
        fr_add(bnum[i - 1], bsgn[i - 1], bden[i - 1], sn, -ss, sd, &dn, &ds, &dd);
        wide_t d = (wide_t)(i + 1);
        if (dn == 0) {
            bnum[i] = 0; bden[i] = 1; bsgn[i] = 1;
        } else {
            wide_t g = w_gcd1(dn, d);
            bnum[i] = dn / g;
            bden[i] = dd * (d / g);
            bsgn[i] = ds;
        }
        have = i + 1;
    }
}

seq_t a(idx_t n) {
    ensure(n < 1 ? 1 : n);
    return (seq_t)bden[n];
}
