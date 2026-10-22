/*
 * A003331  Numbers that are the sum of 8 positive cubes.
 * https://oeis.org/A003331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sum of 8 positive cubes; a(n) = 142 + n for n > 478
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 1000

static seq_t *cached_terms = NULL;
static idx_t cached_count = 0;
static bool initialized = false;

static void compute_sequence(void) {
    if (initialized) return;
    initialized = true;
    
    // Compute all sums of 8 positive cubes up to LIMIT
    bool is_sum[LIMIT + 1];
    for (int i = 0; i <= LIMIT; i++) {
        is_sum[i] = false;
    }
    
    // Iterate through all 8-tuples of positive integers in non-decreasing order
    for (int i0 = 1; i0 * i0 * i0 <= LIMIT; i0++) {
        for (int i1 = i0; i1 * i1 * i1 <= LIMIT; i1++) {
            for (int i2 = i1; i2 * i2 * i2 <= LIMIT; i2++) {
                for (int i3 = i2; i3 * i3 * i3 <= LIMIT; i3++) {
                    for (int i4 = i3; i4 * i4 * i4 <= LIMIT; i4++) {
                        for (int i5 = i4; i5 * i5 * i5 <= LIMIT; i5++) {
                            for (int i6 = i5; i6 * i6 * i6 <= LIMIT; i6++) {
                                for (int i7 = i6; i7 * i7 * i7 <= LIMIT; i7++) {
                                    seq_t sum = (seq_t)(i0*i0*i0) + (seq_t)(i1*i1*i1) + 
                                                (seq_t)(i2*i2*i2) + (seq_t)(i3*i3*i3) +
                                                (seq_t)(i4*i4*i4) + (seq_t)(i5*i5*i5) + 
                                                (seq_t)(i6*i6*i6) + (seq_t)(i7*i7*i7);
                                    if (sum <= LIMIT) {
                                        is_sum[(int)sum] = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Count the number of sums
    idx_t count = 0;
    for (int i = 1; i <= LIMIT; i++) {
        if (is_sum[i]) count++;
    }
    
    // Allocate and populate the cached terms
    cached_terms = (seq_t *)malloc(count * sizeof(seq_t));
    cached_count = count;
    
    idx_t idx = 0;
    for (int i = 1; i <= LIMIT; i++) {
        if (is_sum[i]) {
            cached_terms[idx++] = (seq_t)i;
        }
    }
}

seq_t a(idx_t n) {
    compute_sequence();
    
    if (n <= cached_count) {
        return cached_terms[n - 1];
    } else {
        return (seq_t)142 + (seq_t)n;
    }
}
