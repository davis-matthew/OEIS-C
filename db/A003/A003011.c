/*
 * A003011  Number of permutations of up to n kinds of objects, where each
 *          kind of object can occur at most two times.
 * https://oeis.org/A003011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: n*a(n) = (2*n^3 - n^2 + n + 1)*a(n-1) + (-3*n^3 + 4*n^2 + 2*n - 3)*a(n-2) + (n^3 - 2*n^2 - n + 2)*a(n-3) */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)19;
    
    seq_t *cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)19;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t ni = (seq_t)i;
        seq_t ni2 = ni * ni;
        seq_t ni3 = ni2 * ni;
        
        seq_t c1 = (seq_t)2 * ni3 - ni2 + ni + (seq_t)1;
        seq_t c2 = (seq_t)3 * ni3 - (seq_t)4 * ni2 - (seq_t)2 * ni + (seq_t)3;
        seq_t c3 = ni3 - (seq_t)2 * ni2 - ni + (seq_t)2;
        
        seq_t numerator = c1 * cache[i - 1] + c3 * cache[i - 3] - c2 * cache[i - 2];
        cache[i] = numerator / ni;
    }
    
    seq_t result = cache[n];
    free(cache);
    return result;
}
