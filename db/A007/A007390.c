/*
 * A007390  Number of strict (-1)st-order maximal independent sets in cycle
 *          graph.
 * https://oeis.org/A007390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000204(n) - b(n) where A000204 is the Lucas sequence
 *            L(n) = L(n-1) + L(n-2), L(1) = 1, L(2) = 3 b(1) = 1, b(2k) = 3,
 *            b(2k+1) = 2k+2
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *lucas = NULL;
    static idx_t lucas_len = 0;
    
    if (lucas_len < n) {
        idx_t new_len = (n < 1024) ? 1024 : n + 256;
        seq_t *new_lucas = (seq_t *)malloc(new_len * sizeof(seq_t));
        
        idx_t start_idx = 0;
        if (lucas) {
            for (idx_t i = 0; i < lucas_len; i++) {
                new_lucas[i] = lucas[i];
            }
            free(lucas);
            start_idx = lucas_len;
        } else {
            new_lucas[0] = (seq_t)1;
            new_lucas[1] = (seq_t)3;
            start_idx = 2;
        }
        
        lucas = new_lucas;
        
        for (idx_t i = start_idx; i < new_len; i++) {
            lucas[i] = lucas[i-1] + lucas[i-2];
        }
        lucas_len = new_len;
    }
    
    seq_t L_n = lucas[n - 1];
    
    seq_t b_n;
    if (n == 1) {
        b_n = (seq_t)1;
    } else if (n % 2 == 0) {
        b_n = (seq_t)3;
    } else {
        b_n = (seq_t)(n + 1);
    }
    
    return L_n - b_n;
}
