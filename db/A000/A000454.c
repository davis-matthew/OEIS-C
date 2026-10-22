/*
 * A000454  Unsigned Stirling numbers of first kind s(n,4).
 * https://oeis.org/A000454
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of X^3 in (X+1)(X+2)...(X+n-1), which equals
 *            e_{n-4}, the (n-4)-th elementary symmetric polynomial of {1, 2,
 *            ..., n-1}
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 4) return 0;
    
    idx_t k = n - 4;
    
    seq_t *e = (seq_t *)malloc((k + 1) * sizeof(seq_t));
    if (e == NULL) return 0;
    
    e[0] = 1;
    for (idx_t j = 1; j <= k; j++) {
        e[j] = 0;
    }
    
    for (idx_t i = 1; i < n; i++) {
        seq_t i_seq = (seq_t)i;
        idx_t limit = (k < i ? k : i);
        for (idx_t j = limit; j > 0; j--) {
            e[j] += i_seq * e[j - 1];
        }
    }
    
    seq_t result = e[k];
    free(e);
    return result;
}
