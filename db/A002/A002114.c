/*
 * A002114  Glaisher's H' numbers.
 * https://oeis.org/A002114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, easy, nonn, changed
 * Formula:   a(n) = Sum_{j=1..n-1} (-1)^(j+1)*binomial(2n,2j)*2*a(n-j) +
 *            (-1)^(n+1), a(1) = 1 (from e.g.f. 1/(2*(2*cos(x)-1)), i.e.
 *            2*a(n) = (2n)! [x^(2n)] 1/(2*cos(x)-1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   a001028_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001028_grow.h"

static seq_t *A_memo = NULL;   /* A_memo[k] = a(k), k = 1..A_cnt */
static idx_t A_cnt = 0;
static idx_t A_cap = 0;
static seq_t *R_row = NULL;    /* Pascal row R_m: R_row[i] = C(R_m, i) */
static idx_t R_m = 0;
static idx_t R_cap = 0;

static void pascal_step(void) {
    /* row m -> row m+1, in place; R_row[m+1] is 0 beforehand */
    idx_t i;
    for (i = R_m + 1; i >= 1; i--) {
        R_row[i] += R_row[i - 1];
    }
    R_m++;
}

seq_t a(idx_t n) {
    idx_t k, j, i;
    if (n < 1) return 0;
    if (n <= A_cnt) return A_memo[n];

    if (n + 1 > A_cap) {
        idx_t nc = n + 1;
        A_memo = a001028_grow(A_memo, A_cap, nc);
        for (i = A_cap; i < nc; i++) A_memo[i] = 0;
        A_cap = nc;
    }
    if (2 * n + 2 > R_cap) {
        idx_t nc = 2 * n + 2;
        int fresh = (R_row == NULL);
        R_row = a001028_grow(R_row, R_cap, nc);
        for (i = R_cap; i < nc; i++) R_row[i] = 0;
        R_cap = nc;
        if (fresh) {
            R_row[0] = 1;
            R_m = 0;
        }
    }

    for (k = A_cnt + 1; k <= n; k++) {
        seq_t pos = 0, neg = 0;
        while (R_m < 2 * k) pascal_step();
        for (j = 1; j < k; j++) {
            seq_t t = R_row[2 * j] * A_memo[k - j] * 2;
            if (j & 1) pos += t;
            else neg += t;
        }
        if (k & 1) pos += 1;
        else neg += 1;
        A_memo[k] = pos - neg;
        A_cnt = k;
    }
    return A_memo[n];
}
