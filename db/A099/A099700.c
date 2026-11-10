/*
 * A099700  Consider the family of multigraphs enriched by the species of
 *          simple graphs. Sequence gives number of those multigraphs with n
 *          labeled edges.
 * https://oeis.org/A099700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} b(k) * P(n,k), a(0) = 1, where P(n,k) =
 *            number of partitions of [n] into k blocks, each block of size s
 *            weighted by A006125(s) = 2^(s(s-1)/2), and b = A014500 (graphs
 *            with unlabeled nodes, n labeled edges) = exp-transform: b(0) =
 *            1, b(k) = Sum_{j=1..k} binomial(k-1,j-1)*d(j)*b...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 50 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    int N = (int)n;
    int V = N + 1;                    /* max vertices */
    int MX = V * (V - 1) / 2;         /* max pascal row */
    int PC = N + 1;                   /* pascal columns 0..N */
    int i, j, k, v, m;

    seq_t *pas = mem_new(seq_t, (size_t)(MX + 1) * PC);
    for (i = 0; i <= MX; i++) {
        pas[(size_t)i * PC] = (seq_t)1;
        for (j = 1; j < PC && j <= i; j++)
            pas[(size_t)i * PC + j] = pas[(size_t)(i - 1) * PC + j - 1] + pas[(size_t)(i - 1) * PC + j];
    }
#define A099700_C(x, y) pas[(size_t)(x) * PC + (y)]
#define A099700_G(vv, mm) A099700_C((vv) * ((vv) - 1) / 2, (mm))

    int CS = N + 1;
    seq_t *c = mem_new(seq_t, (size_t)(V + 1) * CS);
    for (v = 1; v <= V; v++) {
        for (m = 0; m <= N; m++) {
            if (m < v - 1) continue;
            seq_t s = (seq_t)0;
            for (k = 1; k <= v - 1; k++) {
                seq_t inner = (seq_t)0;
                int jmax = k * (k - 1) / 2;
                if (jmax > m) jmax = m;
                for (j = k - 1; j <= jmax; j++)
                    inner = inner + c[(size_t)k * CS + j] * A099700_G(v - k, m - j);
                s = s + A099700_C(v - 1, k - 1) * inner;
            }
            c[(size_t)v * CS + m] = A099700_G(v, m) - s;
        }
    }

    seq_t *d = mem_new(seq_t, (size_t)(N + 1));
    if (N >= 1) d[1] = (seq_t)1;
    for (m = 2; m <= N; m++) {
        seq_t s = (seq_t)0;
        for (v = 2; v <= m; v++) {
            seq_t f = (seq_t)1;
            for (i = v + 1; i <= m; i++) f = f * (seq_t)i;
            s = s + c[(size_t)v * CS + m] * f;
        }
        seq_t t = (seq_t)1;
        for (i = 0; i < m - 2; i++) t = t * (seq_t)(m + 1);
        d[m] = s + t;
    }

    seq_t *b = mem_new(seq_t, (size_t)(N + 1));
    b[0] = (seq_t)1;
    for (k = 1; k <= N; k++) {
        seq_t s = (seq_t)0;
        for (j = 1; j <= k; j++) s = s + A099700_C(k - 1, j - 1) * d[j] * b[k - j];
        b[k] = s;
    }

    seq_t *r = mem_new(seq_t, (size_t)(N + 1));
    {
        seq_t cur = (seq_t)1;
        r[0] = (seq_t)1;
        r[1] = cur;
        for (i = 2; i <= N; i++) {
            for (j = 0; j < i - 1; j++) cur = cur + cur;
            r[i] = cur;
        }
    }
    int S2 = N + 1;
    seq_t *P = mem_new(seq_t, (size_t)S2 * S2);
    P[0] = (seq_t)1;
    for (i = 1; i <= N; i++) {
        for (k = 1; k <= i; k++) {
            seq_t s = (seq_t)0;
            for (int sz = 1; sz <= i - k + 1; sz++)
                s = s + A099700_C(i - 1, sz - 1) * r[sz] * P[(size_t)(i - sz) * S2 + (k - 1)];
            P[(size_t)i * S2 + k] = s;
        }
    }
    seq_t res = (seq_t)0;
    for (k = 1; k <= N; k++) res = res + b[k] * P[(size_t)N * S2 + k];
#undef A099700_C
#undef A099700_G
    mem_free(pas);
    mem_free(c);
    mem_free(d);
    mem_free(b);
    mem_free(r);
    mem_free(P);
    return res;
}
