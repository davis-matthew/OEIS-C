/*
 * A002405  Coefficients for step-by-step integration.
 * https://oeis.org/A002405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = lcm{1,...,n+1} * Integral_{x=-1..0} x(x+1)...(x+n-1) dx =
 *            lcm{1,...,n+1} * Sum_{j=0..n} (-1)^j * |S1(n,j)| / (j+1), where
 *            |S1(n,j)| are the unsigned Stirling numbers of the first kind
 *            (the coefficients of the rising factorial x(x+1)...(x+n-1))
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t N = n + 1, i, j, p;
    seq_t *c = (seq_t *)calloc((size_t)(n + 2), sizeof(seq_t));
    unsigned char *comp = (unsigned char *)calloc((size_t)(N + 1), 1);
    idx_t *pe = (idx_t *)calloc((size_t)(N + 1), sizeof(idx_t)); /* max exponent of p with p^e <= N */
    seq_t res = 0;

    for (j = 0; j <= n + 1; j++) c[j] = 0;
    c[0] = 1;
    /* multiply by (x + i) for i = 0..n-1 */
    for (i = 0; i < n; i++) {
        for (j = i + 1; j > 0; j--)
            c[j] = c[j - 1] + c[j] * (seq_t)i;
        c[0] = c[0] * (seq_t)i;
    }

    /* primes up to N */
    for (p = 2; p <= N; p++) {
        if (comp[p]) continue;
        for (i = p * p; i <= N; i += p) comp[i] = 1;
        {
            idx_t e = 0, q = 1;
            while (q <= N / p) { q *= p; e++; }
            pe[p] = e;
        }
    }

    for (j = 0; j <= n; j++) {
        idx_t q = j + 1;
        seq_t m = 1;
        if (c[j] == 0) continue;
        /* m = lcm(1..N) / (j+1), built as a product of prime powers */
        for (p = 2; p <= N; p++) {
            idx_t e, v = 0, t;
            if (comp[p]) continue;
            e = pe[p];
            while (q % p == 0) { q /= p; v++; }
            for (t = v; t < e; t++) m = m * (seq_t)p;
        }
        if (j & 1) res -= c[j] * m;
        else res += c[j] * m;
    }

    free(comp);
    free(pe);
    free(c);
    return res;
}
