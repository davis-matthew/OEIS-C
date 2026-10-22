/*
 * A001829  Related to graded partially ordered sets.
 * https://oeis.org/A001829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p+q+r+s+t+u=n} (n!/(p!q!r!s!t!u!))
 *            2^(pq+qr+rs+st+tu), evaluated by DP over the chain of levels:
 *            T_1(m,j) = [j=m], T_{k+1}(m,j) = C(m,j) * Sum_i T_k(m-j,i)
 *            2^(i*j), a(n) = Sum_j T_6(n,j)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
typedef std::vector<seq_t> a1829_vec;
#define A1829_INIT(v, len) (v).assign((size_t)(len), (seq_t)0)
#define A1829_FREE(v) (v).clear()
#define A1829_SWAP(x, y) (x).swap(y)
#else
typedef seq_t *a1829_vec;
#define A1829_INIT(v, len) ((v) = (seq_t *)calloc((size_t)(len), sizeof(seq_t)))
#define A1829_FREE(v) free(v)
#define A1829_SWAP(x, y) do { seq_t *a1829_tmp = (x); (x) = (y); (y) = a1829_tmp; } while (0)
#endif

seq_t a(idx_t n) {
    size_t N = (size_t)n + 1;
    a1829_vec C, T, U, P2;
    size_t m, j, i, r;
    int level;
    seq_t res;

    A1829_INIT(C, N * N);
    A1829_INIT(T, N * N);
    A1829_INIT(U, N * N);
    A1829_INIT(P2, N);

    /* Pascal triangle */
    for (m = 0; m < N; m++) {
        C[m * N] = 1;
        for (j = 1; j <= m; j++) {
            seq_t v = C[(m - 1) * N + j - 1];
            if (j < m) v += C[(m - 1) * N + j];
            C[m * N + j] = v;
        }
    }

    /* powers of two by doubling */
    P2[0] = 1;
    for (j = 1; j < N; j++) P2[j] = P2[j - 1] + P2[j - 1];

    /* level 1 */
    for (m = 0; m < N; m++) T[m * N + m] = 1;

    for (level = 2; level <= 6; level++) {
        for (m = 0; m < N * N; m++) U[m] = 0;
        for (r = 0; r < N; r++) {
            for (j = 0; r + j < N; j++) {
                seq_t s = 0;
                seq_t cur = 1;
                for (i = 0; i <= r; i++) {
                    s += T[r * N + i] * cur;
                    cur *= P2[j];
                }
                m = r + j;
                U[m * N + j] = C[m * N + j] * s;
            }
        }
        A1829_SWAP(T, U);
    }

    res = 0;
    for (j = 0; j < N; j++) res += T[(N - 1) * N + j];

    A1829_FREE(C);
    A1829_FREE(T);
    A1829_FREE(U);
    A1829_FREE(P2);
    return res;
}
