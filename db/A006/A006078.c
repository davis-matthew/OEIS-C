/*
 * A006078  Number of triangulated (n+2)-gons rooted at an exterior edge.
 * https://oeis.org/A006078
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (C(n+1) - 2*C(n) + [n even]*3*C(n/2)) / 4, where C(k) =
 *            Catalan numbers; computed as 4*a(n) = 2*Sum_{k=1..ceil(n/2)-1}
 *            C(k)*C(n-k) + [n even]*(C(n/2)^2 + 3*C(n/2))
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 991 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

/* C(j) from C(j-1): C(j) = C(j-1) * (4j-2) / (j+1), done without overflow */
static seq_t cat_next(seq_t c, idx_t j) {
    uint64_t den = (uint64_t)j + 1;
    uint64_t num = 4 * (uint64_t)j - 2;
    uint64_t r = (uint64_t)(c % (seq_t)den);
    uint64_t g = gcd_u64(r, den);
    if (g == 0) g = den;
    seq_t cg = c / (seq_t)g;
    uint64_t d2 = den / g;
    return cg * (seq_t)(num / d2);
}

/* C(j-1) from C(j): C(j-1) = C(j) * (j+1) / (4j-2), done without overflow */
static seq_t cat_prev(seq_t c, idx_t j) {
    uint64_t den = 4 * (uint64_t)j - 2;
    uint64_t num = (uint64_t)j + 1;
    uint64_t r = (uint64_t)(c % (seq_t)den);
    uint64_t g = gcd_u64(r, den);
    if (g == 0) g = den;
    seq_t cg = c / (seq_t)g;
    uint64_t d2 = den / g;
    return cg * (seq_t)(num / d2);
}

seq_t a(idx_t n) {
    seq_t c = 1, cm = 0, Q = 0, q = 0, lo, hi, res;
    long long r = 0, rest;
    idx_t j, k, m = n / 2;

    for (j = 1; j <= n - 1; j++) {
        c = cat_next(c, j);
        if (n % 2 == 0 && j == m) cm = c;
    }
    hi = c;          /* C(n-1) */
    lo = 1;          /* C(1) */
    for (k = 1; 2 * k < n; k++) {
        seq_t t = lo * hi;
        q += t / 4;
        r += (long long)(t % 4);
        lo = cat_next(lo, k + 1);
        hi = cat_prev(hi, n - k);
    }
    if (n % 2 == 0) Q = cm * (cm + 3);
    /* 4a = 2*(4q + r) + Q */
    rest = 2 * r + (long long)(Q % 4);
    res = 2 * q + Q / 4 + (seq_t)(rest / 4);
    return res;
}
