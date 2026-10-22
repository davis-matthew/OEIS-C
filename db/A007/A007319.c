/*
 * A007319  A grasshopper sequence: closed under n -> 2n+2 and 6n+6.
 * https://oeis.org/A007319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   The sequence of positive integers that can be generated from 1
 *            by repeatedly applying n -> 2n+2 and n -> 6n+6, sorted in
 *            ascending order
 * Range:     u8 n <= 4, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t capacity = 100000;
    
    for (int trials = 0; trials < 20; trials++) {
        bool *in_set = (bool *)calloc((size_t)(capacity + 1), sizeof(bool));
        
        in_set[1] = true;
        
        for (idx_t i = 1; i <= capacity; i++) {
            if (in_set[(size_t)i]) {
                seq_t i_seq = (seq_t)i;
                seq_t next1 = 2 * i_seq + 2;
                seq_t next2 = 6 * i_seq + 6;
                
                if (next1 < (seq_t)(capacity + 1)) {
                    in_set[(size_t)next1] = true;
                }
                if (next2 < (seq_t)(capacity + 1)) {
                    in_set[(size_t)next2] = true;
                }
            }
        }
        
        idx_t count = 0;
        for (idx_t i = 1; i <= capacity; i++) {
            if (in_set[(size_t)i]) {
                count++;
                if (count == n) {
                    free(in_set);
                    return (seq_t)i;
                }
            }
        }
        
        free(in_set);
        capacity *= 2;
    }
    
    return 0;
}
