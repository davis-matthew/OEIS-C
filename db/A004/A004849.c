/*
 * A004849  Numbers that are the sum of at most 9 positive 5th powers.
 * https://oeis.org/A004849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number expressible as sum of at most 9 positive 5th
 *            powers
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *sequence = NULL;
    static idx_t seq_len = 0;
    
    if (sequence == NULL) {
        long long upper = 1000000;
        
        uint8_t *reach = (uint8_t *)malloc(upper + 1);
        memset(reach, 0, upper + 1);
        reach[0] = 1;
        
        // Generate base values k such that k^5 <= upper
        long long *bases = (long long *)malloc(1000 * sizeof(long long));
        idx_t nbases = 0;
        for (long long k = 1; k <= 100; k++) {
            if (k * k * k * k * k <= upper) {
                bases[nbases++] = k;
            } else {
                break;
            }
        }
        
        // DP: expand reachable set by adding one fifth power at a time (up to 9 times)
        for (int step = 0; step < 9; step++) {
            uint8_t *new_reach = (uint8_t *)malloc(upper + 1);
            memcpy(new_reach, reach, upper + 1);
            
            for (long long i = 0; i <= upper; i++) {
                if (reach[i]) {
                    for (idx_t j = 0; j < nbases; j++) {
                        long long k = bases[j];
                        long long kpow5 = k * k * k * k * k;
                        long long next = i + kpow5;
                        if (next <= upper) {
                            new_reach[next] = 1;
                        }
                    }
                }
            }
            
            memcpy(reach, new_reach, upper + 1);
            free(new_reach);
        }
        
        // Extract the sequence
        idx_t count = 0;
        for (long long i = 0; i <= upper; i++) {
            if (reach[i]) count++;
        }
        
        sequence = (seq_t *)malloc(count * sizeof(seq_t));
        idx_t idx = 0;
        for (long long i = 0; i <= upper; i++) {
            if (reach[i]) {
                sequence[idx++] = (seq_t)i;
            }
        }
        seq_len = count;
        
        free(reach);
        free(bases);
    }
    
    return sequence[n - 1];
}
