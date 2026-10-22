/*
 * A003545  Divisors of 2^39 - 1.
 * https://oeis.org/A003545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   Divisors of 2^39 - 1, where 2^39 - 1 = 7 × 79 × 8191 × 121369
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_seq.h"

seq_t a(idx_t n) {
    if (n < 1 || n > 16) return 0;
    
    seq_t primes[4] = {(seq_t)7, (seq_t)79, (seq_t)8191, (seq_t)121369};
    seq_t divisors[16];
    
    /* Generate all 16 divisors as products of subsets of the four prime factors */
    for (int mask = 0; mask < 16; mask++) {
        seq_t d = (seq_t)1;
        for (int i = 0; i < 4; i++) {
            if (mask & (1 << i)) {
                d = d * primes[i];
            }
        }
        divisors[mask] = d;
    }
    
    qsort(divisors, 16, sizeof(seq_t), cmp_seq);
    
    return divisors[n - 1];
}
