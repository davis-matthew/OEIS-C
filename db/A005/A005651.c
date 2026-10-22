/*
 * A005651  Sum of multinomial coefficients (n_1+n_2+...)!/(n_1!*n_2!*...)
 *          where (n_1, n_2, ...) runs over all integer partitions of n.
 * https://oeis.org/A005651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Sum of multinomial coefficients n!/(n_1!*n_2!*...) over all
 *            integer partitions (n_1, n_2, ...) of n
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   encode
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/encode.h"

static struct {
    seq_t **binom_table;
    seq_t *part_memo;
    idx_t n;
} state = {NULL, NULL, 0};

static seq_t get_binom(idx_t n, idx_t k) {
    if (k > n || k < 0) return 0;
    return state.binom_table[n][k];
}

static seq_t compute_partitions(idx_t remaining, idx_t max_part, idx_t n) {
    if (remaining == 0) {
        return 1;
    }
    
    idx_t index = encode(remaining, max_part, n);
    if (state.part_memo[index] != 0) {
        return state.part_memo[index];
    }
    
    seq_t result = 0;
    for (idx_t part = (remaining < max_part) ? remaining : max_part; part >= 1; part--) {
        seq_t binom_coeff = get_binom(remaining, part);
        seq_t sub_result = compute_partitions(remaining - part, part, n);
        result += binom_coeff * sub_result;
    }
    
    state.part_memo[index] = result;
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    if (state.n < n) {
        if (state.binom_table != NULL) {
            for (idx_t i = 0; i <= state.n; i++) {
                free(state.binom_table[i]);
            }
            free(state.binom_table);
        }
        
        state.binom_table = (seq_t **)malloc((n + 1) * sizeof(seq_t *));
        for (idx_t i = 0; i <= n; i++) {
            state.binom_table[i] = (seq_t *)calloc(i + 1, sizeof(seq_t));
            state.binom_table[i][0] = 1;
            if (i > 0) state.binom_table[i][i] = 1;
            for (idx_t j = 1; j < i; j++) {
                state.binom_table[i][j] = state.binom_table[i-1][j-1] + state.binom_table[i-1][j];
            }
        }
        
        free(state.part_memo);
        idx_t size = (n + 1) * (n + 2);
        state.part_memo = (seq_t *)calloc(size, sizeof(seq_t));
        
        state.n = n;
    }
    
    return compute_partitions(n, n, n);
}
