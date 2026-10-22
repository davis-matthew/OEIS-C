/*
 * A001821  Central factorial numbers: 3rd subdiagonal of A008955.
 * https://oeis.org/A001821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(2*n^2 + 6*n + 7)*a(n-1) - 3*(2*n^4 + 8*n^3 + 16*n^2 +
 *            16*n + 7)*a(n-2) + (2*n^2 + 2*n + 1)*(2*n^4 + 4*n^3 + 6*n^2 +
 *            4*n + 1)*a(n-3) - n^8*a(n-4); a(0) = 1, a(1) = 30, a(2) = 1023,
 *            a(3) = 44473
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t memo_computed = 0;

static void init_memo(void) {
    if (memo == NULL) {
        memo_len = 100;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memset(memo, 0, memo_len * sizeof(seq_t));
        
        memo[0] = 1;
        memo[1] = 30;
        memo[2] = 1023;
        memo[3] = 44473;
        memo_computed = 3;
    }
}

static void ensure_computed(idx_t n) {
    idx_t i;
    
    init_memo();
    
    if (n >= memo_len) {
        idx_t newlen = memo_len * 2;
        while (newlen <= n) newlen *= 2;
        seq_t *newmemo = (seq_t *)malloc(newlen * sizeof(seq_t));
        memcpy(newmemo, memo, memo_len * sizeof(seq_t));
        free(memo);
        memo = newmemo;
        memset(memo + memo_len, 0, (newlen - memo_len) * sizeof(seq_t));
        memo_len = newlen;
    }
    
    for (i = memo_computed + 1; i <= n; i++) {
        seq_t nn = (seq_t)i;
        seq_t nn2 = nn * nn;
        seq_t nn4 = nn2 * nn2;
        seq_t nn8 = nn4 * nn4;
        
        seq_t c1 = 2 * (2 * nn2 + 6 * nn + 7);
        seq_t c2 = 3 * (2 * nn4 + 8 * nn * nn2 + 16 * nn2 + 16 * nn + 7);
        seq_t c3 = (2 * nn2 + 2 * nn + 1) * (2 * nn4 + 4 * nn * nn2 + 6 * nn2 + 4 * nn + 1);
        
        memo[i] = c1 * memo[i-1] - c2 * memo[i-2] + c3 * memo[i-3] - nn8 * memo[i-4];
    }
    memo_computed = (n > memo_computed) ? n : memo_computed;
}

seq_t a(idx_t n) {
    ensure_computed(n);
    return memo[n];
}
