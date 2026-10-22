/*
 * A005833  a(n) = floor(tau*a(n-2)) + a(n-1) with a(0)=0 and a(1)=1.
 * https://oeis.org/A005833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(tau*a(n-2)) + a(n-1), a(0) = 0, a(1) = 1.
 *            floor(tau*x) is computed exactly with integers: - big build:
 *            floor(tau*x) = floor(x*F(k+1)/F(k)) where F(k) > 2x (the
 *            approximation error is smaller than the distance from tau*x to
 *            the nearest integer); - fixed-width build: Zeckendorf x = sum...
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   grow_zero, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_big.h"

static seq_t *F = 0;
static idx_t Flen = 0, Fcap = 0;

static void fib_ensure(idx_t m) {
    if (m <= Flen) return;
    if (m > Fcap) {
        idx_t nc = Fcap ? Fcap * 2 : 64;
        while (nc < m) nc *= 2;
        F = grow_zero(F, Fcap, nc);
        Fcap = nc;
    }
    if (Flen == 0) {
        F[0] = 0;
        F[1] = 1;
        Flen = 2;
    }
    while (Flen < m) {
        F[Flen] = F[Flen - 1] + F[Flen - 2];
        Flen++;
    }
}

static idx_t bigK = 2;

static seq_t floor_tau_big(seq_t x) {
    if (x == 0) return 0;
    fib_ensure(bigK + 2);
    while (F[bigK] <= x * 2) {
        bigK++;
        fib_ensure(bigK + 2);
    }
    return x * F[bigK + 1] / F[bigK];
}

static seq_t floor_tau_fixed(seq_t x) {
    idx_t j, low;
    seq_t r, s;
    if (x == 0) return 0;
    fib_ensure(200);
    j = 2;
    while (j + 1 < 199 && F[j + 1] > F[j] && F[j + 1] <= x) j++;
    r = x;
    s = 0;
    low = j;
    for (;;) {
        if (F[j] <= r) {
            r -= F[j];
            s += F[j + 1];
            low = j;
            if (j < 4) break;
            j -= 2;
        } else {
            if (j <= 2) break;
            j--;
        }
        if (r == 0) break;
    }
    if (low % 2 == 0) s -= 1;
    return s;
}

static seq_t *A = 0;
static idx_t Alen = 0, Acap = 0;

seq_t a(idx_t n) {
    bool big;
    if (n < Alen) return A[n];
    if (n + 1 > Acap) {
        idx_t nc = Acap ? Acap * 2 : 64;
        while (nc < n + 1) nc *= 2;
        A = grow_zero(A, Acap, nc);
        Acap = nc;
    }
    if (Alen == 0) {
        A[0] = 0;
        A[1] = 1;
        Alen = 2;
    }
    big = seq_is_big() != 0;
    while (Alen <= n) {
        seq_t x = A[Alen - 2];
        seq_t f = big ? floor_tau_big(x) : floor_tau_fixed(x);
        A[Alen] = f + A[Alen - 1];
        Alen++;
    }
    return A[n];
}
