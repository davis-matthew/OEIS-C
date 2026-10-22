/*
 * A003546  Divisors of 2^40 - 1.
 * https://oeis.org/A003546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of 2^40 - 1, sorted in increasing order
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 192, u128 n <= 192,
 *            big any n
 * Helpers:   cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_seq.h"

static seq_t *divisors = NULL;
static idx_t divisor_count = 0;

static void compute_divisors(void) {
    if (divisors != NULL) return;
    
    seq_t N = (((seq_t)1 << 20) << 20) - 1;
    divisors = (seq_t *)malloc(256 * sizeof(seq_t));
    
    for (seq_t i = 1; i <= N / i; i++) {
        if (N % i == 0) {
            divisors[divisor_count++] = i;
            seq_t j = N / i;
            if (i != j) {
                divisors[divisor_count++] = j;
            }
        }
    }
    
    qsort(divisors, divisor_count, sizeof(seq_t), cmp_seq);
}

seq_t a(idx_t n) {
    compute_divisors();
    if (n < 1 || (idx_t)n > divisor_count) return 0;
    return divisors[n - 1];
}
