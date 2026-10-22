/*
 * A001832  Number of labeled connected bipartite graphs on n nodes.
 * https://oeis.org/A001832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = b(n)/2 - Sum_{k=1..n-1} binomial(n-1,k-1)*a(k)*b(n-k),
 *            where b(n) = A047863(n) = Sum_{k=0..n}
 *            binomial(n,k)*2^(k*(n-k)), b(0) = 1 (e.g.f. of a = log(e.g.f. of
 *            A047863)/2)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/A001832_pow2.h"

#define A001832_MAXN 10002

static seq_t A001832_A[A001832_MAXN];
static seq_t A001832_B[A001832_MAXN];
static seq_t A001832_row[A001832_MAXN];
static idx_t A001832_N = 0;

seq_t a(idx_t n) {
    if (n < 1 || n >= A001832_MAXN) return 0;
    if (n <= A001832_N) return A001832_A[n];

    if (A001832_N == 0) {
        A001832_B[0] = 1;
        A001832_row[0] = 1; /* row 0 of Pascal's triangle */
    }
    for (idx_t m = A001832_N + 1; m <= n; m++) {
        /* A001832_row currently holds binomial(m-1, .) */
        seq_t S = 0;
        for (idx_t k = 1; k < m; k++)
            S += A001832_row[k - 1] * A001832_A[k] * A001832_B[m - k];

        /* advance row to binomial(m, .) */
        A001832_row[m] = 1;
        for (idx_t k = m - 1; k >= 1; k--)
            A001832_row[k] += A001832_row[k - 1];

        seq_t bfull = 2;   /* k = 0 and k = m terms */
        seq_t bhalf = 1;
        for (idx_t k = 1; k < m; k++) {
            idx_t e = k * (m - k);
            seq_t p = A001832_pow2(e - 1);
            seq_t t = A001832_row[k] * p;
            bhalf += t;
            bfull += t * 2;
        }
        A001832_B[m] = bfull;
        A001832_A[m] = bhalf - S;
    }
    A001832_N = n;
    return A001832_A[n];
}
