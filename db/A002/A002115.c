/*
 * A002115  Generalized Euler numbers.
 * https://oeis.org/A002115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} (-1)^(k+1) * binomial(3*n,3*k) *
 *            a(n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a002115_tab = NULL;
static idx_t a002115_have = 0;
static idx_t a002115_cap = 0;

static void a002115_grow(idx_t need) {
    idx_t newcap = a002115_cap * 2;
    if (newcap < need) newcap = need;
    if (newcap < 16) newcap = 16;
    seq_t *t = (seq_t *)calloc((size_t)newcap, sizeof(seq_t));
    for (idx_t i = 0; i < a002115_have; i++) t[i] = a002115_tab[i];
    if (a002115_tab != NULL) free(a002115_tab);
    a002115_tab = t;
    a002115_cap = newcap;
}

seq_t a(idx_t n) {
    if (n >= a002115_cap) a002115_grow(n + 1);
    if (a002115_have == 0) {
        a002115_tab[0] = 1;
        a002115_have = 1;
    }
    while (a002115_have <= n) {
        idx_t m = a002115_have;
        idx_t N = 3 * m;
        seq_t c = 1;
        seq_t pos = 0, neg = 0;
        for (idx_t j = 1; j <= N; j++) {
            c = c * (seq_t)(N - j + 1) / (seq_t)j; /* c = binomial(N, j), exact */
            if (j % 3 == 0) {
                idx_t k = j / 3;
                seq_t t = c * a002115_tab[m - k];
                if (k % 2 == 1) pos += t;
                else neg += t;
            }
        }
        a002115_tab[m] = pos - neg;
        a002115_have++;
    }
    return a002115_tab[n];
}
