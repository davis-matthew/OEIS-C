/*
 * A000625  Number of n-node steric rooted ternary trees; number of n carbon
 *          alkyl radicals C(n)H(2n+1) taking stereoisomers into account.
 * https://oeis.org/A000625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f. A(x) = 1 + x*(A(x)^3 + 2*A(x^3))/3, i.e. a(0) = 1; a(n) =
 *            ([x^(n-1)] A(x)^3 + 2*a((n-1)/3) if 3 | n-1)/3 for n >= 1. A^2
 *            and A^3 coefficients are maintained incrementally
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 42, u128 n <= 80,
 *            big any n
 * Helpers:   alloc_zero, grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_arr.h"

static seq_t *A_ = NULL, *S2_ = NULL, *S3_ = NULL;
static idx_t cap_ = 0, cnt_ = 0;
static int fixed_ = -1;
static seq_t inv3_;

static void init_mode(void) {
    seq_t t = 1;
    int i;
    for (i = 0; i < 200; i++) t *= 2;
    fixed_ = (t == 0) ? 1 : 0;
    if (fixed_) {
        seq_t v = 3;
        for (i = 0; i < 8; i++) {
            seq_t p = v * 3;
            seq_t c = 2;
            c -= p;
            v = v * c;
        }
        inv3_ = v;
    } else {
        inv3_ = 0;
    }
}

seq_t a(idx_t n) {
    if (fixed_ < 0) init_mode();
    if (n >= cap_) {
        idx_t nc = n + 1 + (n + 1) / 2 + 16;
        if (A_ == NULL) {
            A_ = alloc_zero(nc);
            S2_ = alloc_zero(nc);
            S3_ = alloc_zero(nc);
        } else {
            A_ = grow_arr(A_, cap_, nc);
            S2_ = grow_arr(S2_, cap_, nc);
            S3_ = grow_arr(S3_, cap_, nc);
        }
        cap_ = nc;
    }
    while (cnt_ <= n) {
        idx_t m = cnt_;
        idx_t i;
        if (m == 0) {
            A_[0] = 1;
        } else {
            seq_t num = S3_[m - 1];
            if ((m - 1) % 3 == 0) num += 2 * A_[(m - 1) / 3];
            if (fixed_) A_[m] = num * inv3_;
            else A_[m] = num / 3;
        }
        {
            seq_t s = 0;
            for (i = 0; 2 * i < m; i++) s += A_[i] * A_[m - i];
            s *= 2;
            if (m % 2 == 0) s += A_[m / 2] * A_[m / 2];
            S2_[m] = s;
        }
        {
            seq_t s = 0;
            for (i = 0; i <= m; i++) s += A_[i] * S2_[m - i];
            S3_[m] = s;
        }
        cnt_++;
    }
    return A_[n];
}
