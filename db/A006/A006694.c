/*
 * A006694  Number of cyclotomic cosets of 2 mod 2n+1.
 * https://oeis.org/A006694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 2046, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* a(n) = number of cyclotomic cosets of 2 modulo 2n+1 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    idx_t m = 2 * n + 1;
    
    bool *visited = (bool *)calloc((size_t)m, sizeof(bool));
    
    seq_t count = 0;
    
    for (idx_t i = 1; i < m; i++) {
        if (!visited[i]) {
            count++;
            
            idx_t current = i;
            do {
                visited[current] = true;
                current = (current * 2) % m;
            } while (!visited[current]);
        }
    }
    
    free(visited);
    return count;
}
