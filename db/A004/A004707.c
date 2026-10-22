/*
 * A004707  Expansion of 1/(10 - Sum_{k=1..9} exp(k*x)).
 * https://oeis.org/A004707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n-1} binomial(n,j) * a(j) * s(n-j), a(0) = 1,
 *            where s(m) = Sum_{k=1..9} k^m (from (10 - Sum_{k=1..9} exp(k*x))
 *            * A(x) = 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;   /* a(0..memo_len-1) */
static seq_t *sums = NULL;   /* s(0..memo_len-1) */
static seq_t *row = NULL;    /* binomial(memo_len-1, j), j = 0..memo_len-1 */
static seq_t pw[10];         /* k^(memo_len-1) */
static idx_t memo_len = 0;
static idx_t cap = 0;

static seq_t *grow_buf(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = (seq_t *)malloc((size_t)newlen * sizeof(seq_t));
    idx_t i;
    for (i = 0; i < newlen; i++) p[i] = 0;
    for (i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) free(old);
    return p;
}

seq_t a(idx_t n) {
    idx_t i, j;
    int k;
    if (memo == NULL) {
        cap = 16;
        memo = grow_buf(NULL, 0, cap);
        sums = grow_buf(NULL, 0, cap);
        row = grow_buf(NULL, 0, cap);
        memo[0] = 1;
        sums[0] = 9;
        row[0] = 1;
        for (k = 0; k < 10; k++) pw[k] = 1;
        memo_len = 1;
    }
    if (n >= cap) {
        idx_t nc = cap;
        while (nc <= n) nc *= 2;
        memo = grow_buf(memo, cap, nc);
        sums = grow_buf(sums, cap, nc);
        row = grow_buf(row, cap, nc);
        cap = nc;
    }
    for (i = memo_len; i <= n; i++) {
        seq_t s = 0, result = 0;
        /* advance powers and power sums */
        for (k = 1; k <= 9; k++) {
            pw[k] *= (seq_t)k;
            s += pw[k];
        }
        sums[i] = s;
        /* advance Pascal row from i-1 to i */
        row[i] = 1;
        for (j = i - 1; j >= 1; j--) row[j] += row[j - 1];
        /* row[0] stays 1 */
        for (j = 0; j < i; j++) result += row[j] * memo[j] * sums[i - j];
        memo[i] = result;
        memo_len = i + 1;
    }
    return memo[n];
}
