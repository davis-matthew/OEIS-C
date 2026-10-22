/*
 * A001250  Number of alternating permutations of order n.
 * https://oeis.org/A001250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; for n > 1, a(n) = 2*A000111(n), where
 *            A000111(n) = E(n,n) is read from the Entringer triangle: E(0,0)
 *            = 1, E(i,0) = 0, E(i,k) = E(i,k-1) + E(i-1,i-k) for 1 <= k <= i
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 2) return 1;

    seq_t *prev = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *cur = (seq_t *)calloc(n + 1, sizeof(seq_t));
    prev[0] = 1;

    for (idx_t i = 1; i <= n; i++) {
        cur[0] = 0;
        for (idx_t k = 1; k <= i; k++) {
            cur[k] = cur[k - 1] + prev[i - k];
        }
        seq_t *t = prev;
        prev = cur;
        cur = t;
    }

    seq_t result = prev[n] * 2;
    free(prev);
    free(cur);
    return result;
}
