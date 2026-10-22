/*
 * A006170  Number of factorization patterns of polynomials of degree n over
 *          F_5.
 * https://oeis.org/A006170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Euler transform of b(n) = Sum_{d|n, A001692(d) >= n/d} 1,
 *            where A001692(d) = (1/d) * Sum_{e|d} mu(d/e) * 5^e. Big build:
 *            n*a(n) = Sum_{k=1..n} c(k)*a(n-k), c(k) = Sum_{d|k} d*b(d), a(0)
 *            = 1. Fixed width: multiply out Product_k (1-x^k)^(-b(k))
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 69, u64 n <= 205,
 *            u128 n <= 651, big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"

static int mu_small(uint64_t m)
{
    int r = 1;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            r = -r;
        }
    }
    if (m > 1) r = -r;
    return r;
}

/* number of monic irreducible polynomials of degree d over GF(5) */
static uint64_t irr5(idx_t d)
{
    int64_t s = 0;
    idx_t e;
    if (d > 20) return UINT64_MAX;
    for (e = 1; e <= d; e++) {
        if (d % e == 0) {
            int64_t p = 1;
            idx_t i;
            for (i = 0; i < e; i++) p *= 5;
            s += mu_small(d / e) * p;
        }
    }
    return (uint64_t)(s / (int64_t)d);
}

static uint64_t *bb = NULL, *cc = NULL;
static idx_t bclen = 0;

static void ensure_bc(idx_t n)
{
    idx_t k;
    if (n < bclen) return;
    bb = (uint64_t *)realloc(bb, (size_t)(n + 1) * sizeof(uint64_t));
    cc = (uint64_t *)realloc(cc, (size_t)(n + 1) * sizeof(uint64_t));
    if (bclen == 0) { bb[0] = 0; cc[0] = 0; bclen = 1; }
    for (k = bclen; k <= n; k++) {
        uint64_t cnt = 0, cs = 0;
        idx_t i;
        for (i = 1; i * i <= k; i++) {
            if (k % i == 0) {
                idx_t j = k / i;
                if (irr5(i) >= j) cnt++;
                if (j != i && irr5(j) >= i) cnt++;
            }
        }
        bb[k] = cnt;
        for (i = 1; i * i <= k; i++) {
            if (k % i == 0) {
                idx_t j = k / i;
                cs += (uint64_t)i * bb[i];
                if (j != i) cs += (uint64_t)j * bb[j];
            }
        }
        cc[k] = cs;
    }
    bclen = n + 1;
}

static seq_t *F = NULL;
static idx_t Flen = 0;

static seq_t *G = NULL;
static idx_t Glen = 0, Gcap = 0;

seq_t a(idx_t n)
{
    if (seq_is_big()) {
        idx_t m;
        if (n < Glen) return G[n];
        ensure_bc(n);
        if (n + 1 > Gcap) {
            idx_t nc = Gcap * 2;
            if (nc < n + 1) nc = n + 1;
            G = grow_zero(G, Gcap, nc);
            Gcap = nc;
        }
        if (Glen == 0) { G[0] = 1; Glen = 1; }
        for (m = Glen; m <= n; m++) {
            seq_t s = 0;
            idx_t k;
            for (k = 1; k <= m; k++)
                s += G[m - k] * (seq_t)cc[k];
            G[m] = s / (seq_t)m;
        }
        Glen = n + 1;
        return G[n];
    } else {
        if (n >= Flen) {
            idx_t N = 2 * Flen, k, j;
            seq_t *A;
            if (N < n + 1) N = n + 1;
            ensure_bc(N);
            A = alloc_zero(N);
            A[0] = 1;
            for (k = 1; k < N; k++) {
                uint64_t t;
                for (t = 0; t < bb[k]; t++)
                    for (j = k; j < N; j++)
                        A[j] += A[j - k];
            }
            if (F) release(F, Flen);
            F = A;
            Flen = N;
        }
        return F[n];
    }
}
