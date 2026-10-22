/*
 * A002557  Odd squarefree numbers with an even number of prime factors that
 *          have no prime factors greater than 31.
 * https://oeis.org/A002557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   Odd squarefree numbers with even number of prime factors, all
 *            factors <= 31
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 486, u64 n <= 512, u128 n <= 512,
 *            big any n
 * Helpers:   compare_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compare_seq_t.h"

seq_t a(idx_t n) {
    static const uint64_t primes[] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
    static const int num_primes = 10;
    static seq_t* values = NULL;
    static idx_t count = 0;
    static int initialized = 0;
    
    if (!initialized) {
        seq_t* temp = (seq_t*)malloc(512 * sizeof(seq_t));
        idx_t idx = 0;
        
        for (int mask = 0; mask < (1 << num_primes); mask++) {
            int bit_count = 0;
            uint64_t prod = 1;
            
            for (int i = 0; i < num_primes; i++) {
                if (mask & (1 << i)) {
                    bit_count++;
                    prod *= primes[i];
                }
            }
            
            if (bit_count % 2 == 0) {
                temp[idx++] = (seq_t)prod;
            }
        }
        
        count = idx;
        values = temp;
        qsort(values, count, sizeof(seq_t), compare_seq_t);
        initialized = 1;
    }
    
    return values[n - 1];
}
