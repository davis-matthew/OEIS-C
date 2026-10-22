/*
 * A007723  Triangle a(n,k) of number of M-sequences read by antidiagonals.
 * https://oeis.org/A007723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, tabl
 * Formula:   a(p,n) = Sum_{k=-1..n} Lp(n,k), with Lp(n,k) = Sum_{i=k..n}
 *            L(p-1,n,i)*L(p,i-1,k-1) for p >= 1, 0 <= k < n, Lp(n,n) =
 *            Lp(n,-1) = 1; L0(n,k) = 1 if k = -1 or k = n, else 0. The array
 *            is read by antidiagonals: in row d, the entry at position j (j =
 *            0..d) is a(d-j, j-1)
 * Range:     u8 n <= 48, u16 n <= 71, u32 n <= 127, u64 n <= 199,
 *            u128 n <= 338, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define MAXD 250
#define W (MAXD + 2)
#define MEMOLEN ((MAXD + 1) * (MAXD + 2) / 2)

static seq_t LA[W * W];
static seq_t LB[W * W];
static seq_t memo[MEMOLEN];
static bool have[MEMOLEN];

seq_t a(idx_t n) {
    idx_t d = 0, j;
    long p, nn, q, m, k, i;
    seq_t *prev = LA, *cur = LB, *tmp;

    while ((d + 1) * (d + 2) / 2 <= n) d++;
    j = n - d * (d + 1) / 2;
    if (n < MEMOLEN && have[n]) return memo[n];

    p = (long)(d - j);
    nn = (long)j - 1;

    for (q = 0; q <= p; q++) {
        for (m = -1; m <= nn; m++) {
            seq_t total = 0;
            for (k = -1; k <= m; k++) {
                seq_t v;
                if (k == -1 || k == m) {
                    v = 1;
                } else if (q == 0) {
                    v = 0;
                } else {
                    v = 0;
                    for (i = k; i <= m; i++)
                        v += prev[(m + 1) * W + (i + 1)] * cur[i * W + k];
                }
                cur[(m + 1) * W + (k + 1)] = v;
                total += v;
            }
            {
                long dd = q + m + 1, jj = m + 1;
                long idx = dd * (dd + 1) / 2 + jj;
                if (idx < MEMOLEN) {
                    memo[idx] = total;
                    have[idx] = true;
                }
                if (q == p && m == nn && !(n < MEMOLEN)) return total;
            }
        }
        tmp = prev; prev = cur; cur = tmp;
    }
    return memo[n];
}
