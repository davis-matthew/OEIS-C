/*
 * A001508  a(n) is the number of c-nets with n+1 vertices and 2n+2 edges, n
 *          >= 1.
 * https://oeis.org/A001508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A290326(n+2,n), using the Mullin-Schellenberg generating
 *            function C(x,y) = x^2 y^2 (1/(1+x) + 1/(1+y) - 1 -
 *            (1+u)^2(1+v)^2/(1+u+v)^3), where x = u/(1+v)^2 and y =
 *            v/(1+u)^2. Bivariate Lagrange inversion with i = n-1, j = n+1
 *            gives a(n) = 4*S(2j+1,2i+1,i-1,j-1) - S(2j+2,2i+2,i,j) for n >=
 *            2...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 184 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* exact S(p,q,a,b) split into positive and negative parts (big build) */
static void a1508_S(idx_t p, idx_t q, long long a, idx_t b, seq_t *pos, seq_t *neg)
{
    seq_t t1, t2;
    idx_t l;
    *pos = 0;
    *neg = 0;
    if (a < 0) return;
    t1 = binomial(q, b);
    t2 = binomial(p - 3, (idx_t)a);
    for (l = 0; l <= b; l++) {
        idx_t k = b - l;
        idx_t m = p - 3 - l;
        seq_t c2, term;
        if (m < (idx_t)a) break;
        c2 = (seq_t)((l + 2) * (l + 1) / 2);
        term = c2 * t1 * t2;
        if (l & 1) *neg += term; else *pos += term;
        if (k == 0) break;
        t1 = t1 * (seq_t)k / (seq_t)(q - k + 1);
        if (m == 0) break;
        t2 = t2 * (seq_t)(m - (idx_t)a) / (seq_t)m;
    }
}

static int a1508_is_fixed(void)
{
    seq_t x = 1;
    int t;
    for (t = 0; t < 300; t++) x = x + x;
    return x == 0;
}

static seq_t a1508_mul(seq_t x, seq_t y)
{
    return (seq_t)(x * 1u * y);
}

seq_t a(idx_t n)
{
    idx_t i, j;
    if (n < 2) return 0;
    i = n - 1;
    j = n + 1;
    if (!a1508_is_fixed()) {
        seq_t p1, n1, p2, n2;
        a1508_S(2 * j + 2, 2 * i + 2, (long long)i, j, &p1, &n1);
        a1508_S(2 * j + 1, 2 * i + 1, (long long)i - 1, j - 1, &p2, &n2);
        return (p2 * 4 + n1) - (n2 * 4 + p1);
    } else {
        /* fixed width: everything modulo 2^w via Pascal additions */
        idx_t K = j, R = 2 * n + 1, r, k, l;
        seq_t *row = mem_new(seq_t, (size_t)(K + 1));
        seq_t *q1 = mem_new(seq_t, (size_t)(K + 1));
        seq_t *q2 = mem_new(seq_t, (size_t)(K + 1));
        seq_t *colA = mem_new(seq_t, (size_t)(R + 1));
        seq_t *colB = mem_new(seq_t, (size_t)(R + 1));
        seq_t s1 = 0, s2 = 0, res;
        for (k = 0; k <= K; k++) { row[k] = 0; q1[k] = 0; q2[k] = 0; }
        row[0] = 1;
        for (r = 0; r <= R; r++) {
            if (r > 0) {
                idx_t top = r < K ? r : K;
                for (k = top; k >= 1; k--) row[k] = (seq_t)(row[k] + row[k - 1]);
            }
            colA[r] = row[i];
            colB[r] = row[i - 1];
            if (r == 2 * n) for (k = 0; k <= K; k++) q1[k] = row[k];
            if (r == 2 * n - 1) for (k = 0; k <= K; k++) q2[k] = row[k];
        }
        for (l = 0; l <= j; l++) {
            seq_t c2 = (seq_t)((l + 2) * (l + 1) / 2);
            seq_t term = a1508_mul(a1508_mul(c2, q1[j - l]), colA[2 * j - 1 - l]);
            if (l & 1) s1 = (seq_t)(s1 - term); else s1 = (seq_t)(s1 + term);
        }
        for (l = 0; l <= j - 1; l++) {
            seq_t c2 = (seq_t)((l + 2) * (l + 1) / 2);
            seq_t term = a1508_mul(a1508_mul(c2, q2[j - 1 - l]), colB[2 * j - 2 - l]);
            if (l & 1) s2 = (seq_t)(s2 - term); else s2 = (seq_t)(s2 + term);
        }
        res = (seq_t)(a1508_mul((seq_t)4, s2) - s1);
        mem_free(row); mem_free(q1); mem_free(q2); mem_free(colA); mem_free(colB);
        return res;
    }
}
