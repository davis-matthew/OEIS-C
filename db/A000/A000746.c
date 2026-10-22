/*
 * A000746  Boustrophedon transform of triangular numbers.
 * https://oeis.org/A000746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*E(n-k)*(k+1)*(k+2)/2, where E
 *            = A000111 (Euler zigzag numbers, from the Seidel-Entringer
 *            triangle: T(0,0) = 1, T(m,0) = 0, T(m,k) = T(m,k-1) +
 *            T(m-1,m-k), E(m) = T(m,m)); i.e. A109449(n,k) =
 *            binomial(n,k)*E(n-k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"

static seq_t *eul = NULL;     /* eul[0..have-1] = Euler zigzag numbers */
static seq_t *erow = NULL;    /* Entringer row have-1 */
static seq_t *etmp = NULL;    /* scratch row */
static idx_t have = 0, cap = 0;

static void ensure(idx_t m) {
    idx_t i, k;
    if (m + 1 > cap) {
        idx_t nc = cap * 2;
        seq_t *ne, *nr, *nt;
        if (nc < m + 1) nc = m + 1;
        if (nc < 16) nc = 16;
        ne = alloc_seq(nc);
        nr = alloc_seq(nc);
        nt = alloc_seq(nc);
        for (i = 0; i < have; i++) {
            ne[i] = eul[i];
            nr[i] = erow[i];
        }
        free(eul);
        free(erow);
        free(etmp);
        eul = ne;
        erow = nr;
        etmp = nt;
        cap = nc;
    }
    if (have == 0) {
        erow[0] = 1;
        eul[0] = 1;
        have = 1;
    }
    for (i = have; i <= m; i++) {
        seq_t *sw;
        etmp[0] = 0;
        for (k = 1; k <= i; k++)
            etmp[k] = etmp[k - 1] + erow[i - k];
        eul[i] = etmp[i];
        sw = erow;
        erow = etmp;
        etmp = sw;
        have = i + 1;
    }
}

seq_t a(idx_t n) {
    seq_t c = 1;
    seq_t result = 0;
    idx_t k;
    ensure(n);
    for (k = 0; k <= n; k++) {
        seq_t b = (seq_t)(k + 1) * (seq_t)(k + 2) / 2;
        result += c * eul[n - k] * b;
        if (k < n)
            c = c * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    return result;
}
