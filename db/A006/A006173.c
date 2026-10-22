/*
 * A006173  Witt vector *2!.
 * https://oeis.org/A006173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   inverse Somos transform of A000108 shifted left: Product_{i>=1}
 *            1/(1 - a(i)*x^i) = Sum_{n>=0} A000108(n+1)*x^n, so a(n) =
 *            A000108(n+1) - [x^n] Product_{i=1..n-1} 1/(1 - a(i)*x^i)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"

static seq_t *memo = NULL;
static idx_t memo_cap = 0;

static seq_t catalan(idx_t m) {
    /* C(m) = binom(2m, m) - binom(2m, m+1) */
    return binom(2 * m, m) - binom(2 * m, m + 1);
}

static void build(idx_t N) {
    seq_t *P = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
    seq_t *res = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
    idx_t i, j;
    P[0] = 1;
    for (j = 1; j <= N; j++) P[j] = 0;
    res[0] = 0;
    for (i = 1; i <= N; i++) {
        seq_t ai = catalan(i + 1) - P[i];
        res[i] = ai;
        /* multiply P by 1/(1 - ai*x^i), truncated at degree N */
        for (j = i; j <= N; j++) P[j] += ai * P[j - i];
    }
    free(P);
    if (memo) free(memo);
    memo = res;
    memo_cap = N;
}

seq_t a(idx_t n) {
    if (n > memo_cap) {
        idx_t N = memo_cap * 2;
        if (N < 64) N = 64;
        if (N < n) N = n;
        build(N);
    }
    return memo[n];
}
