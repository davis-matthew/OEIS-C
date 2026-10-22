/*
 * A005034  Number of nonequivalent dissections of a polygon into n
 *          quadrilaterals by nonintersecting diagonals up to rotation.
 * https://oeis.org/A005034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A000139(n) + E(n))/4, where A000139(n) =
 *            2*(3n)!/((2n+1)!*(n+1)!) (this is 3*[z^n]G - [z^n]G^2, using
 *            [z^n]G^2 = (3n+1)/(n+1)*A001764(n)), and E(n) = 2*A001764(n/2)
 *            for n even, E(n) = binom(3m+1,m)/(m+1) [m=(n-1)/2] + (n==1 mod 4
 *            ? 2*A001764((n-1)/4) : 0) for n odd, from G.f. (3G(z) - G(z...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t leg(idx_t N, idx_t p) {
    idx_t e = 0;
    while (N >= p) { N /= p; e += N; }
    return e;
}

/* X = 2^extra2 * A! / (B! * C!), returned as X = 4*h + r, 0 <= r < 4 */
static void quarter_prod(idx_t A, idx_t B, idx_t C, idx_t extra2, seq_t *hout, unsigned *rout) {
    idx_t mx = A;
    if (B > mx) mx = B;
    if (C > mx) mx = C;
    char *comp = (char *)calloc((size_t)mx + 2, 1);
    seq_t h = 0;
    unsigned r = 1;
    idx_t e2 = extra2 + leg(A, 2) - leg(B, 2) - leg(C, 2);
    if (e2 >= 2) {
        h = 1;
        r = 0;
        for (idx_t i = 2; i < e2; i++) h = h * 2;
    } else if (e2 == 1) {
        h = 0;
        r = 2;
    }
    for (idx_t p = 2; p <= mx; p++) {
        if (comp[p]) continue;
        for (idx_t q = p * p; q <= mx; q += p) comp[q] = 1;
        if (p == 2) continue;
        idx_t e = leg(A, p) - leg(B, p) - leg(C, p);
        for (idx_t k = 0; k < e; k++) {
            unsigned long long rp = (unsigned long long)r * (unsigned long long)p;
            h = h * (seq_t)p + (seq_t)(rp / 4);
            r = (unsigned)(rp % 4);
        }
    }
    free(comp);
    *hout = h;
    *rout = r;
}

static seq_t full_value(idx_t A, idx_t B, idx_t C, idx_t extra2) {
    seq_t h;
    unsigned r;
    quarter_prod(A, B, C, extra2, &h, &r);
    return h * 4 + (seq_t)r;
}

/* A001764(m) = (3m)!/(m!*(2m+1)!) */
static seq_t g1(idx_t m) { return full_value(3 * m, m, 2 * m + 1, 0); }
/* [z^m] G^2 = 2*(3m+1)!/(m!*(2m+2)!) */
static seq_t g2(idx_t m) { return full_value(3 * m + 1, m, 2 * m + 2, 1); }

seq_t a(idx_t n) {
    seq_t h;
    unsigned r;
    quarter_prod(3 * n, 2 * n + 1, n + 1, 1, &h, &r);
    seq_t E = 0;
    if (n % 2 == 0) {
        E = g1(n / 2) * 2;
    } else {
        E = g2((n - 1) / 2);
        if (n % 4 == 1) E += g1((n - 1) / 4) * 2;
    }
    return h + ((seq_t)r + E) / 4;
}
