/*
 * A001138  Red rooted red-black trees with n internal nodes.
 * https://oeis.org/A001138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n+1), where T(z) = z^2 + T(z^2*(1+z)^2) counts by
 *            leaves, i.e. T(m) = [m=2] + Sum_{k=ceil(m/4)..floor(m/2)}
 *            T(k)*binomial(2k, m-2k), T(1) = 0. Binomials come from a running
 *            Pascal row (additions only)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A1138_MAX 20010

static seq_t a1138_T[A1138_MAX];
static seq_t a1138_row[A1138_MAX];
static idx_t a1138_done = 0;

static void a1138_build(idx_t N)
{
    idx_t i, j, k;
    for (i = 0; i <= N; i++) {
        a1138_T[i] = 0;
        a1138_row[i] = 0;
    }
    a1138_T[2] = 1;
    a1138_row[0] = 1;          /* Pascal row 0 */
    for (k = 1; 2 * k <= N; k++) {
        idx_t J = (2 * k < N - 2 * k) ? 2 * k : N - 2 * k;
        int step;
        /* advance row 2k-2 -> 2k, keeping entries 0..J */
        for (step = 0; step < 2; step++)
            for (j = J; j >= 1; j--)
                a1138_row[j] += a1138_row[j - 1];
        if (a1138_T[k] != 0) {
            for (j = 0; j <= J; j++)
                a1138_T[2 * k + j] += a1138_T[k] * a1138_row[j];
        }
    }
    a1138_done = N;
}

seq_t a(idx_t n)
{
    idx_t m = n + 1;
    if (n < 1) return 0;
    if (m >= A1138_MAX) return 0;
    if (m > a1138_done) {
        idx_t N = a1138_done * 2;
        if (N < 64) N = 64;
        if (N < m) N = m;
        if (N > A1138_MAX - 1) N = A1138_MAX - 1;
        a1138_build(N);
    }
    return a1138_T[m];
}
