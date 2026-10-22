/*
 * A003080  Number of rooted triangular cacti with 2n+1 nodes (n triangles).
 * https://oeis.org/A003080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen, nice
 * Formula:   a(n) = b(2n+1), where b shifts left under Tb = EULER(E_2(b)),
 *            E_2(b) having g.f. (B(x^2)+B(x)^2)/2. Counted by triangles: A(x)
 *            = Product_{k>=1} (1-x^k)^(-p(k-1)), where p(j) = [x^j] (A(x)^2 +
 *            A(x^2))/2, a(0) = 1. The factors are applied in increasing k:
 *            a(k-1) is final before p(k-1) is needed
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   a3080_gcd, alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/a3080_gcd.h"

static seq_t *a3080_memo = 0;
static idx_t a3080_len = 0;

static void a3080_build(idx_t N) {
    seq_t *f = alloc_zero(N + 1);
    seq_t *av = alloc_zero(N + 1);
    seq_t *B = alloc_zero(N + 1);
    idx_t k, m, i, j;
    f[0] = 1;
    for (k = 1; k <= N; k++) {
        idx_t jj = k - 1, J;
        seq_t c = 0;
        av[k - 1] = f[k - 1];
        /* c = p(jj): number of unordered pairs of cacti with jj triangles in total */
        for (i = 0; 2 * i < jj; i++) c += av[i] * av[jj - i];
        if ((jj & 1) == 0) {
            seq_t h = av[jj / 2];
            if (h % 2 == 0) c += (h / 2) * (h + 1);
            else c += h * ((h + 1) / 2);
        }
        J = N / k;
        /* B[i] = C(c+i-1, i) */
        B[0] = 1;
        for (i = 1; i <= J; i++) {
            seq_t t = c + (seq_t)(i - 1);
            unsigned long long r = (unsigned long long)(B[i - 1] % (seq_t)i);
            unsigned long long g = a3080_gcd(r, (unsigned long long)i);
            if (g == 0) g = (unsigned long long)i;
            B[i] = (B[i - 1] / (seq_t)g) * (t / (seq_t)((unsigned long long)i / g));
        }
        /* f <- f * (1-x^k)^(-c), in place, descending */
        for (m = N; m >= k; m--) {
            seq_t s = f[m];
            for (j = 1; j * k <= m; j++) s += B[j] * f[m - j * k];
            f[m] = s;
        }
    }
    av[N] = f[N];
    free(f);
    free(B);
    if (a3080_memo) free(a3080_memo);
    a3080_memo = av;
    a3080_len = N + 1;
}

seq_t a(idx_t n) {
    if (n >= a3080_len) {
        idx_t N = n;
        if (N < 2 * a3080_len) N = 2 * a3080_len;
        if (N < 16) N = 16;
        a3080_build(N);
    }
    return a3080_memo[n];
}
