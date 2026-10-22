/*
 * A001463  Partial sums of A001462; also a(n) is the last occurrence of n in
 *          A001462.
 * https://oeis.org/A001463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..n} A001462(k), where A001462 is Golomb's
 *            sequence: G(1) = 1, G(k) = 1 + G(k - G(G(k-1))) for k >= 2
 * Range:     u8 n <= 36, u16 n <= 1138, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *golomb_tab = NULL;
static idx_t golomb_len = 0; /* g[1..golomb_len] are computed */

static void golomb_ensure(idx_t n) {
    idx_t i, cap;
    uint64_t *p;
    if (n <= golomb_len) return;
    cap = n + 1;
    if (golomb_len > 0 && cap < 2 * golomb_len) cap = 2 * golomb_len;
    p = (uint64_t *)realloc(golomb_tab, (size_t)(cap + 1) * sizeof(uint64_t));
    if (!p) return;
    golomb_tab = p;
    if (golomb_len == 0) {
        golomb_tab[0] = 0;
        golomb_tab[1] = 1;
        golomb_len = 1;
    }
    for (i = golomb_len + 1; i <= cap; i++) {
        golomb_tab[i] = 1 + golomb_tab[i - golomb_tab[golomb_tab[i - 1]]];
    }
    golomb_len = cap;
}

seq_t a(idx_t n) {
    seq_t s = 0;
    idx_t k;
    if (n < 1) return 0;
    golomb_ensure(n);
    for (k = 1; k <= n; k++) {
        s += (seq_t)golomb_tab[k];
    }
    return s;
}
