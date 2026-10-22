/*
 * A007570  a(n) = F(F(n)), where F is a Fibonacci number.
 * https://oeis.org/A007570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (M(n))[0,1] where M(0) = I, M(1) = [[0, 1], [1, 1]], M(n)
 *            = M(n-2) * M(n-1)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    seq_t m[2][2];
} Matrix;

static Matrix* memo = NULL;
static bool* computed = NULL;
static idx_t memo_size = 0;

static Matrix matrix_mult(Matrix a, Matrix b) {
    Matrix result;
    result.m[0][0] = a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0];
    result.m[0][1] = a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1];
    result.m[1][0] = a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0];
    result.m[1][1] = a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1];
    return result;
}

static void ensure_size(idx_t n) {
    if (n < memo_size) return;
    
    idx_t new_size = n + 1;
    Matrix* new_memo = (Matrix*)malloc(new_size * sizeof(Matrix));
    bool* new_computed = (bool*)malloc(new_size * sizeof(bool));
    
    if (memo != NULL) {
        memcpy(new_memo, memo, memo_size * sizeof(Matrix));
        memcpy(new_computed, computed, memo_size * sizeof(bool));
        free(memo);
        free(computed);
    }
    
    for (idx_t i = memo_size; i < new_size; i++) {
        new_computed[i] = false;
    }
    
    memo = new_memo;
    computed = new_computed;
    memo_size = new_size;
}

static Matrix get_matrix(idx_t n) {
    ensure_size(n);
    
    for (idx_t i = 0; i <= n; i++) {
        if (!computed[i]) {
            if (i == 0) {
                memo[i].m[0][0] = 1; memo[i].m[0][1] = 0;
                memo[i].m[1][0] = 0; memo[i].m[1][1] = 1;
            } else if (i == 1) {
                memo[i].m[0][0] = 0; memo[i].m[0][1] = 1;
                memo[i].m[1][0] = 1; memo[i].m[1][1] = 1;
            } else {
                memo[i] = matrix_mult(memo[i - 2], memo[i - 1]);
            }
            computed[i] = true;
        }
    }
    
    return memo[n];
}

seq_t a(idx_t n) {
    Matrix m = get_matrix(n);
    return m.m[0][1];
}
