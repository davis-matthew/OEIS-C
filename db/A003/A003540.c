/*
 * A003540  Divisors of 2^33 - 1.
 * https://oeis.org/A003540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of 2^33 - 1
 * Range:     u8 none, u16 none, u32 none, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_seq_t(const void *a, const void *b) {
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n) {
    if (sizeof(seq_t) < 8) {
        return 0;
    }
    
    static seq_t divisors[32] = {0};
    static idx_t count = 0;
    static int computed = 0;
    
    if (!computed) {
        seq_t N = ((seq_t)1 << 33) - (seq_t)1;
        
        for (seq_t i = 1; i <= N / i; i++) {
            if (N % i == 0) {
                divisors[count++] = i;
                seq_t j = N / i;
                if (i != j) {
                    divisors[count++] = j;
                }
            }
        }
        
        qsort(divisors, count, sizeof(seq_t), cmp_seq_t);
        computed = 1;
    }
    
    if (n >= 1 && n <= (idx_t)count) {
        return divisors[n - 1];
    }
    
    return 0;
}
