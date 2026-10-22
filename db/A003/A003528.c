/*
 * A003528  Divisors of 2^18 - 1.
 * https://oeis.org/A003528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of 2^18 - 1 = 262143 = 3^3 * 7 * 19 * 73, sorted in
 *            ascending order
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_seq.h"

static seq_t divisors[32];
static int divisor_count = -1;

seq_t a(idx_t n) {
    if (divisor_count == -1) {
        seq_t limit = (seq_t)262143;
        divisor_count = 0;
        
        for (seq_t i = 1; i * i <= limit; i++) {
            if (limit % i == 0) {
                divisors[divisor_count++] = i;
                seq_t other = limit / i;
                if (i != other) {
                    divisors[divisor_count++] = other;
                }
            }
        }
        
        qsort(divisors, divisor_count, sizeof(seq_t), cmp_seq);
    }
    
    return divisors[n - 1];
}
