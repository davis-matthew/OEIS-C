/*
 * A003265  Not representable by truncated tribonacci sequence 2, 4, 7, 13,
 *          24, 44, 81, ....
 * https://oeis.org/A003265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Greedy tribonacci representation; a(n) is the n-th positive
 *            integer whose representation includes 1
 * Range:     u8 n <= 117, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t trib[150];
    int trib_len = 0;
    
    trib[0] = 1;
    trib[1] = 2;
    trib[2] = 4;
    trib_len = 3;
    
    while (trib_len < 150) {
        seq_t next = trib[trib_len - 1] + trib[trib_len - 2] + trib[trib_len - 3];
        if (next < trib[trib_len - 1]) break;
        trib[trib_len++] = next;
    }
    
    seq_t count = 0;
    seq_t m = 0;
    
    while (count < n) {
        m++;
        
        seq_t remainder = m;
        bool uses_1 = false;
        
        for (int i = trib_len - 1; i >= 0; i--) {
            if (trib[i] <= remainder) {
                remainder -= trib[i];
                if (i == 0) uses_1 = true;
            }
        }
        
        if (remainder == 0 && uses_1) {
            count++;
        }
    }
    
    return m;
}
