/*
 * A006477  Number of partitions of n with at least 1 odd and 1 even part.
 * https://oeis.org/A006477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) - A000009(n) if n is odd else A000041(n) -
 *            A000009(n) - A000041(n/2)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 127, u64 n <= 416,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t *p = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *p_odd = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    
    memset(p, 0, (n + 1) * sizeof(seq_t));
    memset(p_odd, 0, (n + 1) * sizeof(seq_t));
    
    p[0] = 1;
    p_odd[0] = 1;
    
    // Compute A000041(n): all partitions
    for (idx_t k = 1; k <= n; k++) {
        for (idx_t i = k; i <= n; i++) {
            p[i] += p[i - k];
        }
    }
    
    // Compute A000009(n): partitions into odd parts
    for (idx_t k = 1; k <= n; k += 2) {
        for (idx_t i = k; i <= n; i++) {
            p_odd[i] += p_odd[i - k];
        }
    }
    
    seq_t result;
    if (n % 2 == 1) {
        result = p[n] - p_odd[n];
    } else {
        seq_t p_half = p[n / 2];
        result = p[n] - p_odd[n] - p_half;
    }
    
    free(p);
    free(p_odd);
    
    return result;
}
