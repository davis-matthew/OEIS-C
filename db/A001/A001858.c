/*
 * A001858  Number of forests of trees on n labeled nodes.
 * https://oeis.org/A001858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, eigen
 * Formula:   a(n) = Sum_{j=0..n-1} C(n-1,j) * (j+1)^(j-1) * a(n-1-j), a(0) =
 *            1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static seq_t power(seq_t base, idx_t exp) {
    if (exp == 0) return 1;
    
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    if (n >= memo_len) {
        idx_t new_len = (n + 1) * 2;
        if (new_len < 1000) new_len = 1000;
        seq_t *new_memo = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        for (idx_t i = memo_len; i < new_len; i++) {
            new_memo[i] = 0;
        }
        memo = new_memo;
        memo_len = new_len;
    }
    
    if (memo[n] != 0) return memo[n];
    
    seq_t result = 0;
    
    for (idx_t j = 0; j < n; j++) {
        seq_t binom_val = binom(n - 1, j);
        seq_t power_val = (j == 0) ? 1 : power((seq_t)(j + 1), j - 1);
        seq_t a_val = a(n - 1 - j);
        result += binom_val * power_val * a_val;
    }
    
    memo[n] = result;
    return result;
}
