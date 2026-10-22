/*
 * A006681  Number of binary phylogenetic trees with n labels.
 * https://oeis.org/A006681
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, changed
 * Formula:   E.g.f. A(x) = (-4 + 3*exp(x) + (3 - 2*exp(x))^(3/2))/3 satisfies
 *            3A' = 2e^x A' - 3e^x A + e^(2x) - e^x, giving a(n+1) =
 *            Sum_{k=0..n-1} C(n,k)*(2*a(k+1) - 3*a(k)) - 3*a(n) + 2^n - 1,
 *            a(0) = 0, a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *av = NULL;   /* av[i] = a(i) for i < cnt */
static seq_t *row = NULL;  /* row[k] = C(cnt-1, k) */
static idx_t cap = 0, cnt = 0;
static seq_t p2;           /* 2^(cnt-1) */

static void ensure(idx_t need) {
    if (need <= cap) return;
    idx_t nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    av = grow_zero(av, cap, nc);
    row = grow_zero(row, cap, nc);
    cap = nc;
}

seq_t a(idx_t n) {
    if (cnt == 0) {
        ensure(2);
        av[0] = 0;
        row[0] = 1;
        p2 = 1;
        cnt = 1;
    }
    while (cnt <= n) {
        idx_t m = cnt - 1;
        ensure(cnt + 2);
        seq_t P = p2;
        seq_t N = av[m] * 3 + 1;
        for (idx_t k = 0; k < m; k++) {
            P += row[k] * (av[k + 1] * 2);
            N += row[k] * (av[k] * 3);
        }
        av[cnt] = P - N;
        row[m + 1] = 1;
        for (idx_t k = m; k >= 1; k--) row[k] += row[k - 1];
        p2 *= 2;
        cnt++;
    }
    return av[n];
}
