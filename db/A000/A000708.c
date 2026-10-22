/*
 * A000708  a(n) = E(n+1) - 2*E(n), where E(i) is the Euler number A000111(i).
 * https://oeis.org/A000708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = E(n+1) - 2*E(n), where E(k) = A000111(k) is computed with
 *            the Seidel-Entringer boustrophedon: T(0,0) = 1, T(k,0) = 0,
 *            T(k,j) = T(k,j-1) + T(k-1,k-j), E(k) = T(k,k)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 14, i64 n <= 22, i128 n <= 37,
 *            big any n
 * Helpers:   zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/zalloc_arr.h"

static seq_t *E_tab = NULL;   /* E_tab[k] = E(k) for k < have */
static seq_t *row = NULL;     /* row (have-1) of the triangle, length have */
static seq_t *tmp = NULL;
static idx_t cap = 0;
static idx_t have = 0;

static void ensure_cap(idx_t need) {
    if (need <= cap) return;
    idx_t newcap = cap * 2;
    if (newcap < need) newcap = need;
    if (newcap < 16) newcap = 16;
    seq_t *ne = zalloc_arr(newcap);
    seq_t *nr = zalloc_arr(newcap);
    seq_t *nt = zalloc_arr(newcap);
    for (idx_t i = 0; i < have; i++) {
        ne[i] = E_tab[i];
        nr[i] = row[i];
    }
    E_tab = ne;
    row = nr;
    tmp = nt;
    cap = newcap;
}

static void extend_to(idx_t m) {
    /* make E(0..m) available */
    ensure_cap(m + 2);
    if (have == 0) {
        E_tab[0] = 1;
        row[0] = 1;
        have = 1;
    }
    while (have <= m) {
        idx_t k = have;
        tmp[0] = 0;
        for (idx_t j = 1; j <= k; j++)
            tmp[j] = tmp[j - 1] + row[k - j];
        seq_t *s = row;
        row = tmp;
        tmp = s;
        E_tab[k] = row[k];
        have++;
    }
}

seq_t a(idx_t n) {
    extend_to(n + 1);
    return E_tab[n + 1] - 2 * E_tab[n];
}
