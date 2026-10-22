/*
 * A004284  Least positive multiple of n written in base 4 using only 0 and 1.
 * https://oeis.org/A004284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 92, u64 n <= 6140,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* BFS search for the least positive multiple of n with only 0, 1 digits in base 4,
   then return its base-4 representation as a decimal number. */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    
    if (n == 1) return 1;
    
    size_t n_size = (size_t)n;
    
    seq_t *queue = (seq_t *)malloc((n_size + 1) * sizeof(seq_t));
    bool *visited = (bool *)malloc(n_size * sizeof(bool));
    
    for (size_t i = 0; i < n_size; i++) visited[i] = false;
    
    size_t queue_head = 0, queue_tail = 0;
    
    queue[queue_tail++] = 1;
    visited[1 % n_size] = true;
    
    while (queue_head < queue_tail) {
        seq_t curr = queue[queue_head++];
        
        seq_t next0 = curr * 4;
        size_t rem0 = (size_t)(next0 % n_val);
        
        if (rem0 == 0) {
            seq_t result = 0;
            seq_t temp = next0;
            seq_t power = 1;
            while (temp > 0) {
                result += (temp % 4) * power;
                power *= 10;
                temp /= 4;
            }
            free(queue);
            free(visited);
            return result;
        }
        
        if (!visited[rem0]) {
            visited[rem0] = true;
            queue[queue_tail++] = next0;
        }
        
        seq_t next1 = curr * 4 + 1;
        size_t rem1 = (size_t)(next1 % n_val);
        
        if (rem1 == 0) {
            seq_t result = 0;
            seq_t temp = next1;
            seq_t power = 1;
            while (temp > 0) {
                result += (temp % 4) * power;
                power *= 10;
                temp /= 4;
            }
            free(queue);
            free(visited);
            return result;
        }
        
        if (!visited[rem1]) {
            visited[rem1] = true;
            queue[queue_tail++] = next1;
        }
    }
    
    free(queue);
    free(visited);
    return 0;
}
