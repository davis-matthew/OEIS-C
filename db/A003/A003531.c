/*
 * A003531  Divisors of 2^22 - 1.
 * https://oeis.org/A003531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 2^22 - 1 = 4194303 = 3 × 23 × 89 × 683
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_seq_t(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

seq_t a(idx_t n) {
    static seq_t divisors[16] = {0};
    static int initialized = 0;
    
    if (!initialized) {
        seq_t primes[4] = {3, 23, 89, 683};
        
        for (int mask = 0; mask < 16; mask++) {
            seq_t divisor = 1;
            for (int i = 0; i < 4; i++) {
                if (mask & (1 << i)) {
                    divisor *= primes[i];
                }
            }
            divisors[mask] = divisor;
        }
        
        qsort(divisors, 16, sizeof(seq_t), cmp_seq_t);
        initialized = 1;
    }
    
    if (n < 1 || n > 16) return 0;
    return divisors[n - 1];
}
