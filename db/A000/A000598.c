/*
 * A000598  Number of rooted ternary trees with n nodes; number of n-carbon
 *          alkyl radicals C(n)H(2n+1) ignoring stereoisomers.
 * https://oeis.org/A000598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, eigen
 * Formula:   a(0) = 1; for n >= 1, a(n) = h3(n-1) with h2(s) = Sum_{i<s-i}
 *            a(i)*a(s-i) + [s even]*binomial(a(s/2)+1, 2), 3*h3(s) =
 *            Sum_{k=0..s} a(k)*h2(s-k) + Sum_{2j<=s} a(j)*a(s-2j) +
 *            [3|s]*a(s/3) (Newton's identity for multisets of 3 children from
 *            A(x), i.e. A(x) = 1 + (1/6)*x*(A(x)^3 + 3*A(x)*A(x^2) + 2*A...
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 49, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A_ = NULL;
static seq_t *H2_ = NULL;
static idx_t cap_ = 0;
static idx_t done_ = 0;
static int fixed_ = -1;
static seq_t inv3_;

static void a598_init(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x *= 2;
    fixed_ = (x == 0) ? 1 : 0;
    if (fixed_) {
        seq_t v = 1;
        for (i = 0; i < 8; i++) {
            seq_t t = (seq_t)3 * v;
            t = (seq_t)2 - t;
            v = (seq_t)(v * t);
        }
        inv3_ = v;
    }
}

static void a598_ensure(idx_t len) {
    if (len <= cap_) return;
    idx_t nc = cap_ * 2;
    if (nc < len) nc = len;
    if (nc < 16) nc = 16;
    A_ = (seq_t *)realloc(A_, nc * sizeof(seq_t));
    H2_ = (seq_t *)realloc(H2_, nc * sizeof(seq_t));
    cap_ = nc;
}

seq_t a(idx_t n) {
    if (fixed_ < 0) a598_init();
    if (n < done_) return A_[n];
    a598_ensure(n + 1);
    for (idx_t m = done_; m <= n; m++) {
        if (m == 0) {
            A_[0] = 1;
            continue;
        }
        idx_t s = m - 1;
        /* h2(s) */
        seq_t h = 0;
        for (idx_t i = 0; 2 * i < s; i++) h += A_[i] * A_[s - i];
        if (s % 2 == 0) {
            seq_t c = A_[s / 2];
            if (c % 2 == 0) h += (c / 2) * (c + 1);
            else h += c * ((c + 1) / 2);
        }
        H2_[s] = h;
        /* 3*h3(s) */
        seq_t t = 0;
        for (idx_t k = 0; k <= s; k++) t += A_[k] * H2_[s - k];
        for (idx_t j = 0; 2 * j <= s; j++) t += A_[j] * A_[s - 2 * j];
        if (s % 3 == 0) t += A_[s / 3];
        if (fixed_) A_[m] = (seq_t)(t * inv3_);
        else A_[m] = t / 3;
    }
    done_ = n + 1;
    return A_[n];
}
