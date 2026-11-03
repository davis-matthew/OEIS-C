/*
 * A001474  w such that w^3+x^3+y^3+z^3=0, w>|x|>|y|>|z|, is soluble.
 * https://oeis.org/A001474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search: w is a term iff w^3 + x^3 + y^3 + z^3 = 0 has a
 *            primitive solution (gcd(w,x,y,z) = 1) with w > |x| > |y| > |z| >
 *            0; for each x a two-pointer scan over cubes looks for y^3 + z^3
 *            = -(w^3 + x^3)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static long long a1474_g(long long u, long long v) {
    if (u < 0) u = -u;
    if (v < 0) v = -v;
    while (v) { long long t = u % v; u = v; v = t; }
    return u;
}

static bool a1474_ok(long long w) {
    long long w3 = w * w * w;
    long long X;
    int s;
    for (X = 3; X < w; X++) {
        long long X3 = X * X * X;
        for (s = 0; s < 2; s++) {
            long long T = s ? -(w3 + X3) : -(w3 - X3);
            long long lo = -(X - 1), hi = X - 1;
            while (lo <= hi) {
                long long v = lo * lo * lo + hi * hi * hi;
                if (v == T) {
                    long long al = lo < 0 ? -lo : lo;
                    long long ah = hi < 0 ? -hi : hi;
                    if (al != ah && al != 0 && ah != 0 &&
                        a1474_g(a1474_g(w, X), a1474_g(al, ah)) == 1)
                        return true;
                    lo++;
                    hi--;
                } else if (v < T) {
                    lo++;
                } else {
                    hi--;
                }
            }
        }
    }
    return false;
}

static long long *a1474_terms = 0;
static idx_t a1474_cnt = 0, a1474_cap = 0;
static long long a1474_lastw = 0;

seq_t a(idx_t n) {
    while (a1474_cnt < n) {
        long long w = ++a1474_lastw;
        if (a1474_ok(w)) {
            if (a1474_cnt == a1474_cap) {
                idx_t nc = a1474_cap ? a1474_cap * 2 : 64;
                a1474_terms = (long long *)realloc(a1474_terms, (size_t)nc * sizeof(long long));
                a1474_cap = nc;
            }
            a1474_terms[a1474_cnt++] = w;
        }
    }
    return (seq_t)a1474_terms[n - 1];
}
