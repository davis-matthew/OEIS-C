/*
 * A007371  Numbers k such that sigma(x) = k has exactly 2 solutions.
 * https://oeis.org/A007371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   k such that sigma(x) = k has exactly 2 solutions
 * Range:     u8 n <= 21, u16 n <= 2489, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static uint64_t sigma(uint64_t x) {
    uint64_t sum = 0;
    for (uint64_t i = 1; i * i <= x; i++) {
        if (x % i == 0) {
            sum += i;
            if (i != x / i) {
                sum += x / i;
            }
        }
    }
    return sum;
}

static int cmp_u64(const void *a, const void *b) {
    uint64_t ua = *(const uint64_t *)a;
    uint64_t ub = *(const uint64_t *)b;
    if (ua < ub) return -1;
    if (ua > ub) return 1;
    return 0;
}

seq_t a(idx_t n) {
    idx_t search_limit = 100 + 50 * (idx_t)n;
    if (search_limit < 5000) search_limit = 5000;
    if (search_limit > 2000000) search_limit = 2000000;
    
    uint64_t *sigmas = (uint64_t *)malloc(search_limit * sizeof(uint64_t));
    
    for (idx_t x = 1; x <= search_limit; x++) {
        sigmas[x-1] = sigma(x);
    }
    
    qsort(sigmas, search_limit, sizeof(uint64_t), cmp_u64);
    
    idx_t count_found = 0;
    for (idx_t i = 0; i < search_limit; ) {
        uint64_t val = sigmas[i];
        int occurrences = 0;
        idx_t j = i;
        while (j < search_limit && sigmas[j] == val) {
            occurrences++;
            j++;
        }
        
        if (occurrences == 2) {
            count_found++;
            if (count_found == n) {
                seq_t result = (seq_t)val;
                free(sigmas);
                return result;
            }
        }
        
        i = j;
    }
    
    free(sigmas);
    return (seq_t)0;
}
