/*
 * A060897  Number of walks of length n on square lattice, starting at origin,
 *          staying in first and third quadrants.
 * https://oeis.org/A060897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: 1 + 2*(B(x)-1)/(2 - C(x^2)), B = g.f. of A005566, C = g.f.
 *            of A005568. With D(x)=1/(2-C(x^2)): d(0)=1, d(m)=sum_{k>=1}
 *            c(k)*d(m-2k); a(0)=1, a(n)=2*sum_{j=1..n} b(j)*d(n-j)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 717 (2026-11-07)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    idx_t N = n;
    idx_t i, j, k;
    seq_t *c = mem_new(seq_t, N + 1);
    seq_t *d = mem_new(seq_t, N + 1);
    /* c(k) = Catalan(k)*Catalan(k+1) for 2k <= N */
    seq_t cat = 1;
    for (k = 0; 2 * k <= N; k++) {
        seq_t cat1 = cat * (seq_t)(2 * (2 * k + 1)) / (seq_t)(k + 2);
        c[k] = cat * cat1;
        cat = cat1;
    }
    d[0] = 1;
    for (i = 1; i <= N; i++) {
        seq_t s = 0;
        for (k = 1; 2 * k <= i; k++) {
            s += c[k] * d[i - 2 * k];
        }
        d[i] = s;
    }
    seq_t total = 0;
    for (j = 1; j <= N; j++) {
        seq_t bj = binomial(j, j / 2) * binomial(j + 1, (j + 1) / 2);
        total += bj * d[N - j];
    }
    mem_free(c);
    mem_free(d);
    return total * 2;
}
