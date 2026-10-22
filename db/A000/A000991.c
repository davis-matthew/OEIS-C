/*
 * A000991  Number of 3-line partitions of n.
 * https://oeis.org/A000991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. (1-x)^2*(1-x^2) / Product_{k>=1} (1-x^k)^3, so a(n) = b(n)
 *            - 2*b(n-1) + 2*b(n-3) - b(n-4), with b = A000716. b(0) = 1 and
 *            b(n) = Sum_{m>=1} (-1)^(m+1)*(2m+1)*b(n - m(m+1)/2) (Jacobi:
 *            Product (1-x^k)^3 = Sum_{m>=0} (-1)^m (2m+1) x^(m(m+1)/2))
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 61, u64 n <= 181, u128 n <= 585,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *B = NULL;
static idx_t B_cap = 0, B_have = 0;

static void ensure(idx_t N) {
    if (N + 1 > B_cap) {
        idx_t nc = 2 * B_cap;
        if (nc < N + 1) nc = N + 1;
        if (nc < 64) nc = 64;
        seq_t *nb = alloc_zero(nc);
        for (idx_t i = 0; i < B_have; i++) nb[i] = B[i];
        if (B) release(B, B_cap);
        B = nb;
        B_cap = nc;
    }
    for (idx_t i = B_have; i <= N; i++) {
        if (i == 0) { B[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (idx_t m = 1; m * (m + 1) / 2 <= i; m++) {
            idx_t t = m * (m + 1) / 2;
            seq_t term = (seq_t)(2 * m + 1) * B[i - t];
            if (m & 1) pos += term; else neg += term;
        }
        B[i] = pos - neg;
    }
    if (N + 1 > B_have) B_have = N + 1;
}

seq_t a(idx_t n) {
    ensure(n);
    seq_t pos = B[n];
    seq_t neg = 0;
    if (n >= 1) neg += (seq_t)2 * B[n - 1];
    if (n >= 3) pos += (seq_t)2 * B[n - 3];
    if (n >= 4) neg += B[n - 4];
    return pos - neg;
}
