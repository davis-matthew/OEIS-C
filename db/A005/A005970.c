/*
 * A005970  Partial sums of squares of Lucas numbers.
 * https://oeis.org/A005970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L(n)*L(n+1) - 2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *lucas = NULL;
static idx_t lucas_len = 0;

static void compute_lucas(idx_t up_to) {
    if (lucas_len > up_to) return;
    
    idx_t new_len = (up_to + 1 < 100) ? 100 : up_to + 10;
    seq_t *new_lucas = (seq_t *)malloc(sizeof(seq_t) * new_len);
    
    idx_t start = 0;
    if (lucas) {
        for (idx_t i = 0; i < lucas_len; i++) {
            new_lucas[i] = lucas[i];
        }
        free(lucas);
        start = lucas_len;
    } else {
        new_lucas[0] = 2;
        if (new_len > 1) new_lucas[1] = 1;
        start = 2;
    }
    
    for (idx_t i = start; i < new_len; i++) {
        new_lucas[i] = new_lucas[i-1] + new_lucas[i-2];
    }
    
    lucas = new_lucas;
    lucas_len = new_len;
}

seq_t a(idx_t n) {
    compute_lucas(n + 1);
    return lucas[n] * lucas[n + 1] - 2;
}
