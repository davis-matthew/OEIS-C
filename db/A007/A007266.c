/*
 * A007266  McKay-Thompson series of class 9A for Monster.
 * https://oeis.org/A007266
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^(n+1)] eta(q^3)^12/(eta(q)^6*eta(q^9)^6) - 6*[n=0],
 *            computed as J(q^3)^4 / (J(q)^2 * J(q^9)^2) with J(q) =
 *            prod(1-q^k)^3 = Sum_{m>=0} (-1)^m (2m+1) q^(m(m+1)/2)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 37, u64 n <= 136, u128 n <= 506,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

/* each coefficient is stored as a signed pair P[i] - Q[i] */
static seq_t *P = NULL, *Q = NULL;
static idx_t capM = -1;

static void addterm(seq_t *pp, seq_t *mm, idx_t c, int neg, idx_t j)
{
    seq_t x = P[j] * (seq_t)c;
    seq_t y = Q[j] * (seq_t)c;
    if (!neg) { *pp += x; *mm += y; }
    else      { *pp += y; *mm += x; }
}

static void norm(seq_t *p, seq_t *m)
{
    if (*p >= *m) { *p -= *m; *m = (seq_t)0; }
    else          { *m -= *p; *p = (seq_t)0; }
}

/* multiply in place by J(q^d) */
static void mulJ(idx_t d, idx_t M)
{
    idx_t i, m, t;
    for (i = M; i >= 0; i--) {
        seq_t pp = (seq_t)0, mm = (seq_t)0;
        for (m = 0; (t = d * m * (m + 1) / 2) <= i; m++)
            addterm(&pp, &mm, 2 * m + 1, (int)(m & 1), i - t);
        norm(&pp, &mm);
        P[i] = pp; Q[i] = mm;
    }
}

/* divide in place by J(q^d) */
static void divJ(idx_t d, idx_t M)
{
    idx_t i, m, t;
    for (i = 0; i <= M; i++) {
        seq_t pp = P[i], mm = Q[i];
        for (m = 1; (t = d * m * (m + 1) / 2) <= i; m++)
            addterm(&pp, &mm, 2 * m + 1, (int)((m & 1) == 0), i - t);
        norm(&pp, &mm);
        P[i] = pp; Q[i] = mm;
    }
}

static void compute(idx_t M)
{
    int k;
    P = alloc_zero(M + 1);
    Q = alloc_zero(M + 1);
    P[0] = (seq_t)1;
    for (k = 0; k < 4; k++) mulJ(3, M);
    for (k = 0; k < 2; k++) divJ(1, M);
    for (k = 0; k < 2; k++) divJ(9, M);
    capM = M;
}

seq_t a(idx_t n)
{
    idx_t need = n + 1;
    if (n < -1) return (seq_t)0;
    if (n == 0) return (seq_t)0;
    if (need > capM) {
        idx_t M = need;
        if (capM >= 0) {
            if (2 * capM > M) M = 2 * capM;
            release(P, capM + 1);
            release(Q, capM + 1);
        }
        if (M < 64) M = 64;
        compute(M);
    }
    return P[need] - Q[need];
}
