/*
 * A002029  Number of connected graphs on n labeled nodes, each node being
 *          colored with one of 4 colors, such that no edge joins nodes of the
 *          same color.
 * https://oeis.org/A002029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n) - Sum_{k=1..n-1} binomial(n-1,k-1)*a(k)*b(n-k) for n
 *            >= 1, a(0) = 1, where b(n) = Sum_{k=0..n}
 *            binomial(n,k)*2^(k*(n-k))*c(k)*c(n-k) counts 4-colored graphs
 *            and c(m) = Sum_{j=0..m} binomial(m,j)*2^(j*(m-j)) counts
 *            2-colored graphs (E.g.f.: log(B(x)) + 1, i.e. the logarithmic
 *            transform)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   a2029_two_pow, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/zalloc_arr.h"
#include "../utils/a2029_two_pow.h"

static seq_t *a2029_A = NULL;    /* a(m) */
static seq_t *a2029_B4 = NULL;   /* 4-colored graphs */
static seq_t *a2029_B2 = NULL;   /* 2-colored graphs */
static seq_t *a2029_row = NULL;  /* Pascal row */
static idx_t a2029_have = 0;     /* number of terms computed */
static idx_t a2029_cap = 0;

static seq_t *a2029_regrow(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = zalloc_arr(newlen);
    for (idx_t i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) free(old);
    return p;
}

static void a2029_reserve(idx_t need) {
    if (need <= a2029_cap) return;
    idx_t nc = a2029_cap * 2;
    if (nc < need) nc = need;
    if (nc < 16) nc = 16;
    a2029_A = a2029_regrow(a2029_A, a2029_have, nc);
    a2029_B4 = a2029_regrow(a2029_B4, a2029_have, nc);
    a2029_B2 = a2029_regrow(a2029_B2, a2029_have, nc);
    a2029_row = a2029_regrow(a2029_row, a2029_have, nc);
    a2029_cap = nc;
}

seq_t a(idx_t n) {
    if (n < a2029_have) return a2029_A[n];
    a2029_reserve(n + 1);
    for (idx_t m = a2029_have; m <= n; m++) {
        /* sum for a(m) uses Pascal row m-1 (currently stored) */
        seq_t s = 0;
        for (idx_t k = 1; k < m; k++)
            s += a2029_row[k - 1] * a2029_A[k] * a2029_B4[m - k];
        /* advance Pascal row to row m */
        a2029_row[m] = 1;
        for (idx_t k = m; k-- > 1; )
            a2029_row[k] += a2029_row[k - 1];
        a2029_row[0] = 1;
        /* 2-colored graphs */
        seq_t c = 0;
        for (idx_t j = 0; j <= m; j++)
            c += a2029_row[j] * a2029_two_pow(j * (m - j));
        a2029_B2[m] = c;
        /* 4-colored graphs: split colors into {1,2} and {3,4} */
        seq_t b = 0;
        for (idx_t k = 0; k <= m; k++)
            b += a2029_row[k] * a2029_two_pow(k * (m - k)) * a2029_B2[k] * a2029_B2[m - k];
        a2029_B4[m] = b;
        if (m == 0) a2029_A[m] = 1;
        else a2029_A[m] = b - s;
        a2029_have = m + 1;
    }
    return a2029_A[n];
}
