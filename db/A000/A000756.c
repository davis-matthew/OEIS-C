/*
 * A000756  Boustrophedon transform of sequence 1,1,0,0,0,0,...
 * https://oeis.org/A000756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000111(n) + n*A000111(n-1) for n > 0, a(0) = 1, where
 *            A000111 (Euler zigzag numbers) is computed with the
 *            Seidel-Entringer triangle: T(0,0) = 1, T(m,0) = 0, T(m,k) =
 *            T(m,k-1) + T(m-1,m-k), A000111(m) = T(m,m)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A756_MAXN 20002

static seq_t a756_E[A756_MAXN];
static seq_t a756_R[2][A756_MAXN];
static idx_t a756_done = 0;
static int a756_init = 0;

static void a756_extend(idx_t n) {
    if (!a756_init) {
        a756_R[0][0] = 1;
        a756_E[0] = 1;
        a756_done = 0;
        a756_init = 1;
    }
    while (a756_done < n) {
        idx_t m = a756_done;
        seq_t *prv = a756_R[m & 1];
        seq_t *cur = a756_R[(m + 1) & 1];
        idx_t k;
        cur[0] = 0;
        for (k = 1; k <= m + 1; k++) {
            cur[k] = cur[k - 1] + prv[m + 1 - k];
        }
        a756_E[m + 1] = cur[m + 1];
        a756_done = m + 1;
    }
}

seq_t a(idx_t n) {
    seq_t r;
    if (n == 0) return 1;
    if (n >= A756_MAXN) n = A756_MAXN - 1;
    a756_extend(n);
    r = a756_E[n];
    r += (seq_t)n * a756_E[n - 1];
    return r;
}
