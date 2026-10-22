/*
 * A006849  Number of strongly self-dual planar maps with 2n edges.
 * https://oeis.org/A006849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(2k) = 3^(2k)*C(2k)/2, a(2k-1) = (3^(2k-1)*C(2k-1) +
 *            3^(k-1)*C(k-1))/2, where C(n) = A000108(n) (Catalan numbers),
 *            i.e. (A005159(2k-1)+A005159(k-1))/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define SMALL_MAX 200

static seq_t mul(seq_t x, seq_t y) {
    return x * (y + 0u);
}

static seq_t pow3(idx_t e) {
    seq_t r = 1;
    idx_t i;
    for (i = 0; i < e; i++) r = mul(r, (seq_t)3);
    return r;
}

/* (3^e - 1)/2 computed without division */
static seq_t half3m1(idx_t e) {
    seq_t h = 0;
    idx_t i;
    for (i = 0; i < e; i++) h = mul(h, (seq_t)3) + 1;
    return h;
}

/* C[i] = Catalan(i), H[i] = floor(Catalan(i)/2), exact modulo the width of seq_t */
static void catalan_halves(idx_t m, seq_t *C, seq_t *H) {
    idx_t j, i;
    C[0] = 1;
    H[0] = 0;
    for (j = 1; j <= m; j++) {
        seq_t S = 0;
        for (i = 0; 2 * i + 1 < j; i++) S += mul(C[i], C[j - 1 - i]);
        if (j % 2 == 1) {
            idx_t h = (j - 1) / 2;
            seq_t sq = mul(C[h], C[h]);
            seq_t hs = mul(C[h], H[h]);
            if (C[h] % 2 == 1) hs += H[h];
            C[j] = S + S + sq;
            H[j] = S + hs;
        } else {
            C[j] = S + S;
            H[j] = S;
        }
    }
}

/* 3^m * Catalan(m) via exact multiplicative recurrence */
static seq_t b005159(idx_t m) {
    seq_t b = 1;
    idx_t i;
    for (i = 1; i <= m; i++) {
        b = mul(b, (seq_t)(6 * (2 * i - 1)));
        b /= (seq_t)(i + 1);
    }
    return b;
}

seq_t a(idx_t n) {
    if (n <= SMALL_MAX) {
        seq_t C[SMALL_MAX + 1], H[SMALL_MAX + 1];
        catalan_halves(n, C, H);
        if (n % 2 == 0) {
            return mul(pow3(n), H[n]);
        } else {
            idx_t k = (n + 1) / 2;
            seq_t p2 = pow3(k - 1);
            seq_t r = mul(pow3(n), H[n]) + mul(p2, H[k - 1]);
            if (C[n] % 2 == 1 && C[k - 1] % 2 == 1)
                r += mul(p2, half3m1(k) + 1);
            return r;
        }
    } else {
        if (n % 2 == 0) {
            return b005159(n) / 2;
        } else {
            idx_t k = (n + 1) / 2;
            return (b005159(n) + b005159(k - 1)) / 2;
        }
    }
}
