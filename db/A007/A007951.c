/*
 * A007951  Ternary sieve: delete every 3rd number, then every 9th, 27th, etc.
 * https://oeis.org/A007951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 9826, u32 n <= 9826, u64 n <= 9826,
 *            u128 n <= 9826, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Ternary sieve: find the n-th integer surviving deletion of every 3^k-th element (k≥1) */

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t m = 0;
    
    while (count < (seq_t)n) {
        m++;
        
        seq_t pos = m;
        seq_t power = 3;
        bool survives = true;
        
        while (power <= pos) {
            if (pos % power == 0) {
                survives = false;
                break;
            }
            pos = pos - pos / power;
            power *= 3;
        }
        
        if (survives) {
            count++;
        }
    }
    
    return m;
}
