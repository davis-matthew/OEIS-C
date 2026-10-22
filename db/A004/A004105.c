/*
 * A004105  Number of point-self-dual nets with 2n nodes. Also number of
 *          directed 2-multigraphs with loops on n nodes.
 * https://oeis.org/A004105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{1*s_1+2*s_2+...=n}
 *            fixA[s_1,s_2,...]/(1^s_1*s_1!*2^s_2*s_2!*...), fixA[s_1,s_2,...]
 *            = 3^Sum_{i,j>=1} gcd(i,j)*s_i*s_j. Computed exactly as a(n) =
 *            (1/n!) * Sum (n!/z(s)) * 3^e(s), keeping every quantity as a
 *            pair (q, r) meaning q*n! + r with 0 <= r < n!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t F;              /* n! */
static seq_t tot_q, tot_r;   /* running sum = tot_q*F + tot_r */
static idx_t *parts, *mult;  /* distinct parts and their multiplicities */
static idx_t nparts;

static idx_t g_gcd(idx_t a, idx_t b) {
    while (b) { idx_t t = a % b; a = b; b = t; }
    return a;
}

/* (q1*F + r1) * (q2*F + r2) -> (*q)*F + *r */
static void pmul(seq_t q1, seq_t r1, seq_t q2, seq_t r2, seq_t *q, seq_t *r) {
    seq_t rr = r1 * r2;
    seq_t nq = q1 * q2 * F + q1 * r2 + q2 * r1 + rr / F;
    *r = rr % F;
    *q = nq;
}

static void pow3_pair(idx_t e, seq_t *q, seq_t *r) {
    seq_t one = 1, three = 3;
    seq_t rq = one / F, rr = one % F;
    seq_t bq = three / F, br = three % F;
    while (e > 0) {
        if (e & 1) pmul(rq, rr, bq, br, &rq, &rr);
        e >>= 1;
        if (e > 0) pmul(bq, br, bq, br, &bq, &br);
    }
    *q = rq;
    *r = rr;
}

static void contrib(void) {
    idx_t e = 0;
    seq_t z = 1, c, Q, R, t;
    idx_t i, j, k;
    for (i = 0; i < nparts; i++) {
        e += parts[i] * mult[i] * mult[i];
        for (j = i + 1; j < nparts; j++)
            e += 2 * g_gcd(parts[i], parts[j]) * mult[i] * mult[j];
        for (k = 1; k <= mult[i]; k++)
            z = z * (seq_t)parts[i] * (seq_t)k;
    }
    c = F / z;
    pow3_pair(e, &Q, &R);
    t = c * R;
    tot_q += c * Q + t / F;
    tot_r += t % F;
    if (tot_r >= F) { tot_r -= F; tot_q++; }
}

static void gen(idx_t rem, idx_t maxp) {
    idx_t p, m;
    if (rem == 0) { contrib(); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        idx_t maxm = rem / p;
        parts[nparts] = p;
        nparts++;
        for (m = 1; m <= maxm; m++) {
            mult[nparts - 1] = m;
            gen(rem - m * p, p - 1);
        }
        nparts--;
    }
}

seq_t a(idx_t n) {
    idx_t i;
    if (n == 0) return 1;
    F = 1;
    for (i = 2; i <= n; i++) F = F * (seq_t)i;
    tot_q = 0;
    tot_r = 0;
    parts = (idx_t *)calloc(n + 1, sizeof(idx_t));
    mult = (idx_t *)calloc(n + 1, sizeof(idx_t));
    nparts = 0;
    gen(n, n);
    free(parts);
    free(mult);
    return tot_q;
}
