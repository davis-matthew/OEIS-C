/*
 * A007788  Number of augmented André 3-signed permutations: E.g.f.
 *          (1-sin(3*x))^(-1/3).
 * https://oeis.org/A007788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   y = (1-sin(3x))^(-1/3) satisfies y'' = 5*y^4 - y, so a(n+2) =
 *            5*[x^n/n!]y^4 - a(n), with a(0) = 1, a(1) = 1; y^2 and y^4
 *            coefficients by binomial (EGF) convolution
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *A_ = NULL, *S2_ = NULL, *S4_ = NULL, *C_ = NULL;
static idx_t cap_ = 0;     /* allocated length of arrays */
static idx_t lenA_ = 0;    /* number of a-values computed */
static idx_t rowm_ = 0;    /* C_ holds binomial row rowm_ (valid if lenA_ > 2) */

static seq_t *regrow(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = alloc_zero(newlen);
    idx_t i;
    for (i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) release(old, oldlen);
    return p;
}

static void ensure_cap(idx_t need) {
    idx_t nc;
    if (need <= cap_) return;
    nc = cap_ ? cap_ : 64;
    while (nc < need) nc *= 2;
    A_ = regrow(A_, cap_, nc);
    S2_ = regrow(S2_, cap_, nc);
    S4_ = regrow(S4_, cap_, nc);
    C_ = regrow(C_, cap_, nc);
    cap_ = nc;
}

/* EGF convolution coefficient m of f*f using binomial row C_ (row m) */
static seq_t self_conv(const seq_t *f, idx_t m) {
    seq_t s = 0;
    idx_t j;
    for (j = 0; 2 * j < m; j++)
        s += C_[j] * f[j] * f[m - j];
    s *= 2;
    if (m % 2 == 0)
        s += C_[m / 2] * f[m / 2] * f[m / 2];
    return s;
}

seq_t a(idx_t n) {
    ensure_cap(n + 3);
    if (lenA_ == 0) {
        A_[0] = 1;
        A_[1] = 1;
        lenA_ = 2;
    }
    while (lenA_ <= n) {
        idx_t m = lenA_ - 2;
        idx_t j;
        /* advance binomial row to row m */
        if (m == 0) {
            C_[0] = 1;
        } else {
            C_[m] = 1;
            for (j = m - 1; j >= 1; j--)
                C_[j] += C_[j - 1];
        }
        rowm_ = m;
        S2_[m] = self_conv(A_, m);
        S4_[m] = self_conv(S2_, m);
        A_[m + 2] = 5 * S4_[m] - A_[m];
        lenA_++;
    }
    return A_[n];
}
