/*
 * A001560  Numbers with an even number of partitions.
 * https://oeis.org/A001560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th number m such that p(m) is even, where p(m) is
 *            the partition count of m
 * Range:     u8 n <= 109, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static int *parity_cache = NULL;
static idx_t parity_cache_size = 0;

static int get_partition_parity(idx_t m) {
    if (m >= parity_cache_size) {
        idx_t new_size = m + 1;
        int *new_cache = (int *)malloc(new_size * sizeof(int));
        if (!new_cache) return 0;
        
        memset(new_cache, 0, new_size * sizeof(int));
        new_cache[0] = 1;
        
        for (idx_t k = 1; k <= m; k++) {
            for (idx_t i = k; i <= m; i++) {
                new_cache[i] = (new_cache[i] + new_cache[i - k]) & 1;
            }
        }
        
        if (parity_cache) free(parity_cache);
        parity_cache = new_cache;
        parity_cache_size = new_size;
    }
    
    return parity_cache[m];
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m = 0;
    
    while (count < n) {
        m++;
        if (get_partition_parity(m) == 0) {
            count++;
        }
    }
    
    return (seq_t)m;
}
