/*
 * A225558  a(n) = A003418(n)/A000793(n).
 * https://oeis.org/A225558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003418(n)/A000793(n); computed via prime exponents: a(n)
 *            = prod over primes p<=n of p^(floor(log_p n) - e_p), where e_p
 *            is the exponent of p in a Landau-optimal prime-power partition
 *            of n (0/1 knapsack with backtracking)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 30, u64 n <= 60, u128 n <= 108,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 994 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n < 2) return 1;
    size_t N = (size_t)n;
    bool *comp = (bool *)calloc(N + 1, sizeof(bool));
    size_t *pr = (size_t *)malloc((N + 1) * sizeof(size_t));
    size_t m = 0, p, i, s;
    for (p = 2; p <= N; p++) {
        if (comp[p]) continue;
        pr[m++] = p;
        for (i = p * p; i <= N; i += p) comp[i] = true;
    }
    seq_t *g = mem_new(seq_t, N + 1);
    unsigned char *choice = (unsigned char *)calloc(m * (N + 1), 1);
    for (s = 0; s <= N; s++) g[s] = 1;
    for (i = 0; i < m; i++) {
        p = pr[i];
        for (s = N; s >= p; s--) {
            seq_t best = g[s];
            unsigned char bk = 0;
            size_t pk = p;
            unsigned char k = 1;
            while (pk <= s) {
                seq_t cand = g[s - pk] * (seq_t)pk;
                if (cand > best) { best = cand; bk = k; }
                if (pk > N / p) break;
                pk *= p;
                k++;
            }
            g[s] = best;
            choice[i * (N + 1) + s] = bk;
        }
    }
    int *ex = (int *)calloc(m, sizeof(int));
    s = N;
    for (i = m; i-- > 0;) {
        int k = choice[i * (N + 1) + s];
        if (k) {
            ex[i] = k;
            size_t pk = 1;
            for (int j = 0; j < k; j++) pk *= pr[i];
            s -= pk;
        }
    }
    seq_t result = 1;
    for (i = 0; i < m; i++) {
        p = pr[i];
        int L = 0;
        size_t pk = p;
        for (;;) {
            L++;
            if (pk > N / p) break;
            pk *= p;
        }
        int d = L - ex[i];
        for (int j = 0; j < d; j++) result = result * (seq_t)p;
    }
    free(ex);
    free(choice);
    mem_free(g);
    free(pr);
    free(comp);
    return result;
}
