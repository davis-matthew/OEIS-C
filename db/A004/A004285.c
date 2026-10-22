/*
 * A004285  Least positive multiple of n written in base 5 using only 0 and 1.
 * https://oeis.org/A004285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Least positive multiple of n written in base 5 using only 0 and
 *            1
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 123, u64 n <= 6247,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    idx_t queue_size = n + 100;
    idx_t *queue_rem = (idx_t *)malloc(sizeof(idx_t) * queue_size);
    seq_t *queue_repr = (seq_t *)malloc(sizeof(seq_t) * queue_size);
    bool *visited = (bool *)malloc(sizeof(bool) * n);
    memset(visited, false, sizeof(bool) * n);
    
    idx_t front = 0, back = 0;
    
    idx_t r = 1 % n;
    queue_rem[back] = r;
    queue_repr[back] = 1;
    visited[r] = true;
    back++;
    
    while (front < back) {
        idx_t rem = queue_rem[front];
        seq_t repr = queue_repr[front];
        front++;
        
        if (rem == 0) {
            free(queue_rem);
            free(queue_repr);
            free(visited);
            return repr;
        }
        
        idx_t next_rem = (rem * 5) % n;
        if (!visited[next_rem]) {
            visited[next_rem] = true;
            queue_rem[back] = next_rem;
            queue_repr[back] = repr * 10;
            back++;
        }
        
        next_rem = (rem * 5 + 1) % n;
        if (!visited[next_rem]) {
            visited[next_rem] = true;
            queue_rem[back] = next_rem;
            queue_repr[back] = repr * 10 + 1;
            back++;
        }
    }
    
    free(queue_rem);
    free(queue_repr);
    free(visited);
    return 0;
}
