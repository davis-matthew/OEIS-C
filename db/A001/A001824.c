/*
 * A001824  Central factorial numbers: 1st subdiagonal of A008956.
 * https://oeis.org/A001824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*(4*n^2+1)*a(n-1) - (2*n-1)^4*a(n-2), a(0) = 1, a(1) =
 *            10
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static struct {
    seq_t *data;
    idx_t len;
    idx_t computed;
} state = {NULL, 0, 0};

static void ensure_computed(idx_t n) {
    if (state.data == NULL) {
        state.len = (n < 100) ? 100 : n + 10;
        state.data = (seq_t *)malloc(state.len * sizeof(seq_t));
        state.data[0] = 1;
        state.data[1] = 10;
        state.computed = 2;
    }
    
    if (n >= state.len) {
        idx_t new_len = n + 10;
        state.data = (seq_t *)realloc(state.data, new_len * sizeof(seq_t));
        state.len = new_len;
    }
    
    for (idx_t i = state.computed; i <= n; i++) {
        seq_t coeff1 = 2 * (4 * (seq_t)i * (seq_t)i + 1);
        seq_t coeff2 = (seq_t)(2 * i - 1);
        coeff2 = coeff2 * coeff2 * coeff2 * coeff2;
        state.data[i] = coeff1 * state.data[i - 1] - coeff2 * state.data[i - 2];
    }
    
    state.computed = n + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 10;
    ensure_computed(n);
    return state.data[n];
}
