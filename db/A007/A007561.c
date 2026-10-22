/*
 * A007561  Number of asymmetric rooted connected graphs where every block is
 *          a complete graph.
 * https://oeis.org/A007561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(n+1) = WEIGH(WEIGH(a))(n), a(0) = 0, a(1) = 1, where
 *            WEIGH(u)(n) = [x^n] Prod_{k>=1} (1+x^k)^u(k). Arbitrary
 *            precision: n*w(n) = Sum_{k=1..n} e(k)*w(n-k), e(k) = Sum_{d|k}
 *            (-1)^(k/d+1)*d*u(d). Fixed width: the products are expanded
 *            directly with exact binomials
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 42, u128 n <= 79,
 *            big any n
 * Helpers:   alloc_zero, ecomp, gcd_seq, grow_zero, release,
 *            seq_is_fixed_width, wstep
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/gcd_seq.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/ecomp.h"
#include "../utils/wstep.h"

/* P(x) *= (1+x^k)^m, truncated at degree N */
static void mulfac(seq_t *P, idx_t N, idx_t k, seq_t m, seq_t *bin)
{
    idx_t J = N / k, j, i;
    bool zero = false;
    bin[0] = 1;
    for (j = 1; j <= J; j++) {
        if (zero || m < (seq_t)j) { zero = true; bin[j] = 0; continue; }
        {
            seq_t C = bin[j - 1];
            seq_t g = gcd_seq(C, (seq_t)j);
            seq_t q = (seq_t)j / g;
            bin[j] = (C / g) * ((m - (seq_t)j + 1) / q);
        }
    }
    for (i = N; i >= k; i--) {
        seq_t s = 0;
        for (j = 1; j * k <= i; j++)
            if (bin[j] != 0) s += bin[j] * P[i - j * k];
        P[i] += s;
        if (i == k) break;
    }
}

static seq_t small_a(idx_t n)
{
    idx_t N, m;
    seq_t *B, *C, *bin, r;
    if (n == 0) return 0;
    N = n - 1;
    B = alloc_zero(N + 1);
    C = alloc_zero(N + 1);
    bin = alloc_zero(N + 2);
    B[0] = 1; C[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t am = C[m - 1];
        mulfac(B, N, m, am, bin);
        mulfac(C, N, m, B[m], bin);
    }
    r = C[N];
    release(B, N + 1); release(C, N + 1); release(bin, N + 2);
    return r;
}

static seq_t *A, *Bv, *Cv, *AP, *AN, *BP, *BN;
static idx_t cnt = 0, cap = 0;

seq_t a(idx_t n)
{
    idx_t m;
    if (seq_is_fixed_width() && n <= 200) return small_a(n);
    if (n == 0) return 0;
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 64;
        while (nc < n + 1) nc *= 2;
        A = grow_zero(A, cap, nc);
        Bv = grow_zero(Bv, cap, nc);
        Cv = grow_zero(Cv, cap, nc);
        AP = grow_zero(AP, cap, nc);
        AN = grow_zero(AN, cap, nc);
        BP = grow_zero(BP, cap, nc);
        BN = grow_zero(BN, cap, nc);
        cap = nc;
    }
    if (cnt == 0) {
        A[0] = 0; Bv[0] = 1; Cv[0] = 1;
        AP[0] = 0; AN[0] = 0; BP[0] = 0; BN[0] = 0;
        cnt = 1;
    }
    for (m = cnt; m <= n; m++) {
        A[m] = Cv[m - 1];
        ecomp(A, m, AP, AN);
        Bv[m] = wstep(Bv, AP, AN, m);
        ecomp(Bv, m, BP, BN);
        Cv[m] = wstep(Cv, BP, BN, m);
        cnt = m + 1;
    }
    return A[n];
}
