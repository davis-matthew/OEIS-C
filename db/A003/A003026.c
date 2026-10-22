/*
 * A003026  Number of n-node labeled acyclic digraphs with 2 out-points.
 * https://oeis.org/A003026
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n,2), where T(n,k) = number of labeled acyclic digraphs
 *            on n nodes with exactly k out-points (equivalently, k sources):
 *            T(k,k) = 1, T(n,k) = binomial(n,k) * Sum_{t=1..n-k} (2^k-1)^t *
 *            2^(k*(n-k-t)) * T(n-k,t) for n > k
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"

static seq_t **T_tab = NULL;
static seq_t **C_tab = NULL;
static idx_t T_done = 0;
static idx_t T_cap = 0;

static void ensure_rows(idx_t N) {
    idx_t m, s, j, t;
    if (T_tab == NULL) {
        T_cap = N + 1;
        T_tab = (seq_t **)malloc(T_cap * sizeof(seq_t *));
        C_tab = (seq_t **)malloc(T_cap * sizeof(seq_t *));
        C_tab[0] = alloc_zero(1);
        C_tab[0][0] = 1;
        T_tab[0] = alloc_zero(1);
        T_done = 0;
    }
    if (N <= T_done) return;
    if (N + 1 > T_cap) {
        T_cap = N + 1;
        T_tab = (seq_t **)realloc(T_tab, T_cap * sizeof(seq_t *));
        C_tab = (seq_t **)realloc(C_tab, T_cap * sizeof(seq_t *));
    }
    for (m = T_done + 1; m <= N; m++) {
        /* Pascal row (additive, so exact modulo the word size) */
        C_tab[m] = alloc_zero(m + 1);
        C_tab[m][0] = 1;
        C_tab[m][m] = 1;
        for (j = 1; j < m; j++)
            C_tab[m][j] = C_tab[m - 1][j - 1] + C_tab[m - 1][j];

        T_tab[m] = alloc_zero(m + 1);
        {
            seq_t q = 1;
            for (s = 1; s <= m; s++) {
                q = q * 2; /* q = 2^s */
                if (s == m) {
                    T_tab[m][s] = 1;
                } else {
                    seq_t base = q - 1;
                    idx_t L = m - s;
                    seq_t acc = 0;
                    seq_t bt = 1;
                    /* Horner: acc = Sum_{t=1..L} T(L,t) * base^t * q^(L-t) */
                    for (t = 1; t <= L; t++) {
                        bt = bt * base;
                        acc = acc * q + T_tab[L][t] * bt;
                    }
                    T_tab[m][s] = C_tab[m][s] * acc;
                }
            }
        }
    }
    T_done = N;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    ensure_rows(n);
    return T_tab[n][2];
}
