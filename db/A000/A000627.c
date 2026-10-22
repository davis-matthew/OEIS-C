/*
 * A000627  Number of non-stereoisomeric paraffins with n carbon atoms.
 * https://oeis.org/A000627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   count alkane carbon trees (every carbon has at most 4
 *            neighbours) in which no carbon carries 4 constitutionally
 *            distinct substituents, i.e. no stereocentre. Count them by
 *            rooting each tree at its centroid. r(k) counts the allowed
 *            rooted trees with k carbons and at most 3 children: r(1) = 1,
 *            r(k)...
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   grow_arr, tri_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_arr.h"
#include "../utils/tri_s.h"

static seq_t *R = 0;
static idx_t Rlen = 0;

static void ensure(idx_t N)
{
    idx_t k, s;
    if (N < Rlen) return;
    R = grow_arr(R, Rlen, N + 1);
    for (k = Rlen; k <= N; k++) {
        seq_t v = 0;
        if (k == 1) v = 1;
        else if (k >= 2) {
            v = R[k - 1];
            if ((k - 1) % 2 == 0) v += R[(k - 1) / 2];
            for (s = 1; 2 * s + 1 <= k - 1; s++)
                v += R[s] * R[k - 1 - 2 * s];
        }
        R[k] = v;
    }
    Rlen = N + 1;
}

/* multisets of 2 rooted trees, sizes in [1,M], total size m */
static seq_t m2(idx_t m, idx_t M)
{
    seq_t v = 0;
    idx_t i;
    if (m < 2) return v;
    for (i = 1; 2 * i < m; i++) {
        if (m - i > M) continue;
        v += R[i] * R[m - i];
    }
    if (m % 2 == 0 && m / 2 <= M) v += tri_s(R[m / 2]);
    return v;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t M, m, s;
    if (n <= 1) return (seq_t)1;
    ensure(n);
    M = (n - 1) / 2;
    m = n - 1;

    /* root degree 2 */
    total += m2(m, M);

    /* root degree 3: {A,A,B} */
    for (s = 1; s <= M && 2 * s < m; s++) {
        idx_t t = m - 2 * s;
        if (t <= M) total += R[s] * R[t];
    }

    /* root degree 4: at least one repeated branch */
    for (s = 1; s <= M && 2 * s + 2 <= m; s++)
        total += R[s] * m2(m - 2 * s, M);
    if (m % 2 == 0) {
        idx_t h = m / 2;
        seq_t e2 = m2(h, M);
        if (h % 2 == 0 && h / 2 >= 1 && h / 2 <= M) e2 -= R[h / 2];
        total -= e2;
    }

    /* bicentroid */
    if (n % 2 == 0) total += tri_s(R[n / 2]);

    return total;
}
