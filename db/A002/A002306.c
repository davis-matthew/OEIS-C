/*
 * A002306  Numerators of Hurwitz numbers H_n (coefficients in expansion of
 *          Weierstrass P-function).
 * https://oeis.org/A002306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, frac
 * Formula:   Hurwitz recurrence from P'^2 = 4P^3 - 4P: H(1) = 1/10, H(n) =
 *            3/((4n-1)(4n+1)(2n-3)) * Sum_{i=1..n-1}
 *            (4i-1)(4n-4i-1)*C(4n,4i)*H(i)*H(n-i) for n >= 2; a(n) =
 *            H(n)*A047817(n), where A047817(n) = 2*Prod_{p prime, p == 1 (mod
 *            4), (p-1) | 4n} p (Hurwitz)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   hz_bstep, hz_is_fixed, hz_isprime, hz_pf_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/hz_bstep.h"
#include "../utils/hz_is_fixed.h"
#include "../utils/hz_isprime.h"
#include "../utils/hz_pf_next.h"

typedef unsigned __int128 hz_u128;

static hz_u128 hz_inv(hz_u128 d)
{
    hz_u128 y = d;
    int k;
    for (k = 0; k < 7; k++) y = y * (2 - d * y);
    return y;
}

/* ---------- fixed-width path: 2-adic arithmetic mod 2^128 ---------- */
static hz_u128 *fh = NULL, *frow = NULL;
static idx_t fdone = 0, fcap = 0;

static void hz_fstep(hz_u128 *row, idx_t r)
{
    idx_t k;
    row[r] = 0;
    for (k = r; k > 0; k--) row[k] += row[k - 1];
}

static seq_t hz_fixed_calc(idx_t n)
{
    idx_t m, i;
    if (n > fcap) {
        idx_t nc = n > 2 * fcap ? n : 2 * fcap;
        fh = (hz_u128 *)realloc(fh, (size_t)(nc + 1) * sizeof(hz_u128));
        frow = (hz_u128 *)realloc(frow, (size_t)(4 * nc + 1) * sizeof(hz_u128));
        if (fcap == 0) frow[0] = 1;
        fcap = nc;
    }
    for (m = fdone + 1; m <= n; m++) {
        idx_t r = 4 * (m - 1);
        hz_u128 mid, S, den;
        hz_fstep(frow, ++r);
        hz_fstep(frow, ++r);
        hz_fstep(frow, ++r);
        mid = frow[2 * m - 1];          /* C(4m-1,2m-1) = C(4m,2m)/2 */
        hz_fstep(frow, ++r);
        if (m == 1) { fh[1] = hz_inv(5); continue; }   /* h = 2H */
        S = 0;
        for (i = 1; 2 * i < m; i++)
            S += (hz_u128)(4 * i - 1) * (hz_u128)(4 * m - 4 * i - 1) * frow[4 * i] * fh[i] * fh[m - i];
        if (m % 2 == 0) {
            idx_t j = m / 2;
            S += (hz_u128)(2 * m - 1) * (hz_u128)(2 * m - 1) * mid * fh[j] * fh[j];
        }
        den = (hz_u128)(4 * m - 1) * (hz_u128)(4 * m + 1) * (hz_u128)(2 * m - 3);
        fh[m] = 3 * S * hz_inv(den);
    }
    if (n > fdone) fdone = n;
    {
        hz_u128 res = fh[n];
        uint64_t d = 4, p;
        seq_t out, sh;
        while ((p = hz_pf_next(n, &d)) != 0) res *= (hz_u128)p;
        out = (seq_t)(uint64_t)(res >> 64);
        sh = (seq_t)(uint64_t)4294967296ULL;
        out = out * sh;
        out = out * sh;
        out += (seq_t)(uint64_t)res;
        return out;
    }
}

/* ---------- arbitrary-precision path: t(i) = M * 2H(i) are integers ---------- */
static seq_t *bt = NULL;
static idx_t bN = 0;
static seq_t bM;

static void hz_big_build(idx_t N)
{
    seq_t *row, M;
    idx_t m, i, r = 0;
    uint64_t p;
    M = 1;
    for (p = 5; p <= 4 * N + 1; p += 4)
        if (hz_isprime(p)) M *= (seq_t)p;
    if (bt) free(bt);
    bt = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    row = (seq_t *)calloc((size_t)(4 * N + 1), sizeof(seq_t));
    row[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t mid, S, den;
        hz_bstep(row, ++r);
        hz_bstep(row, ++r);
        hz_bstep(row, ++r);
        mid = row[2 * m - 1];
        hz_bstep(row, ++r);
        if (m == 1) { bt[1] = M / 5; continue; }
        S = 0;
        for (i = 1; 2 * i < m; i++) {
            seq_t c = (seq_t)(4 * i - 1) * (seq_t)(4 * m - 4 * i - 1);
            S += c * row[4 * i] * bt[i] * bt[m - i];
        }
        if (m % 2 == 0) {
            idx_t j = m / 2;
            seq_t c = (seq_t)(2 * m - 1) * (seq_t)(2 * m - 1);
            S += c * mid * bt[j] * bt[j];
        }
        den = (seq_t)(4 * m - 1) * (seq_t)(4 * m + 1) * (seq_t)(2 * m - 3);
        S = S * 3;
        S = S / den;
        bt[m] = S / M;
    }
    free(row);
    bM = M;
    bN = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (hz_is_fixed()) return hz_fixed_calc(n);
    if (n > bN) hz_big_build(n);
    {
        seq_t res = bt[n];
        uint64_t d = 4, p;
        while ((p = hz_pf_next(n, &d)) != 0) res *= (seq_t)p;
        return res / bM;
    }
}
