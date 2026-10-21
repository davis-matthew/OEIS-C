/*
 * A052620  E.g.f. (1-x)^2/(1-3x+x^3).
 * https://oeis.org/A052620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=8; for n≥3 * a(n)=3*n*a(n‑1) –
 *            n*(n‑1)*(n‑2)*a(n‑3). * This follows from
 *            (1‑3x+x³)·(∑a(n)xⁿ/n!)=(1‑x)²
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static size_t memo_len = 0;
static size_t memo_cap = 0;

static void reserve_mem(idx_t needed) {
    if (needed < memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap * 2 : 8;
    while (new_cap <= (size_t)needed) new_cap *= 2;
    seq_t *new_mem = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!new_mem) abort();          /* allocation failed */
    memo = new_mem;
    memo_cap = new_cap;
}

seq_t a(idx_t n) {
    /* Initialise table with known values */
    if (!memo) {
        reserve_mem(3);
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)1;
        memo[2] = (seq_t)8;
        memo_len = 3;
    }

    if (n < memo_len) return memo[n];

    /* Extend table up to n */
    reserve_mem(n);
    for (idx_t i = memo_len; i <= n; ++i) {
        if (i < 3) {             /* should not happen, safety */
            memo[i] = (i==0)?(seq_t)1:
                       (i==1)?(seq_t)1:
                       (seq_t)8;
            continue;
        }
        seq_t term1 = memo[i-1];
        term1 = term1 * (seq_t)3 * (seq_t)i;          /* 3*i*a(i-1) */

        seq_t prod = (seq_t)i * (seq_t)(i-1) * (seq_t)(i-2);
        seq_t term3 = memo[i-3] * prod;               /* i*(i-1)*(i-2)*a(i-3) */

        memo[i] = term1 - term3;                      /* a(i) = term1 - term3 */
    }
    memo_len = n + 1;
    return memo[n];
}
