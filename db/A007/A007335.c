/*
 * A007335  MU-numbers: next term is uniquely the product of 2 earlier terms.
 * https://oeis.org/A007335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   n-th positive integer of the form 2^v*3^w where v=1 or w=1 or
 *            both v,w odd
 * Range:     u8 n <= 12, u16 n <= 33, u32 n <= 107, u64 n <= 375,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *hamming = NULL;
    static idx_t hamming_len = 0;
    
    idx_t target_factor = 3;
    
    while (1) {
        if (hamming_len < n * target_factor) {
            if (hamming != NULL) free(hamming);
            hamming_len = n * target_factor + 100;
            hamming = (seq_t *)malloc((hamming_len + 1) * sizeof(seq_t));
            hamming[0] = 1;
            
            idx_t i2 = 0, i3 = 0;
            for (idx_t i = 1; i <= hamming_len; i++) {
                seq_t next2 = hamming[i2] * 2;
                seq_t next3 = hamming[i3] * 3;
                seq_t next = next2 < next3 ? next2 : next3;
                hamming[i] = next;
                if (next == next2) i2++;
                if (next == next3) i3++;
            }
        }
        
        idx_t filtered_count = 0;
        for (idx_t i = 1; i <= hamming_len; i++) {
            seq_t val = hamming[i];
            
            seq_t v = 0, w = 0;
            seq_t temp = val;
            while (temp % 2 == 0) { v++; temp /= 2; }
            while (temp % 3 == 0) { w++; temp /= 3; }
            
            if (v == 1 || w == 1 || ((v & 1) && (w & 1))) {
                filtered_count++;
                if (filtered_count == n) {
                    return val;
                }
            }
        }
        
        target_factor *= 2;
    }
}
