/*
 * A001476  Numbers that are not the sum of distinct positive cubes.
 * https://oeis.org/A001476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Find the n-th number that cannot be expressed as sum of distinct
 *            positive cubes
 * Range:     u8 n <= 27, u16 n <= 2788, u32 n <= 2788, u64 n <= 2788,
 *            u128 n <= 2788, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>

#define MAX_VAL 20000

static bool can_sum[MAX_VAL + 1];
static bool computed = false;

static void precompute() {
    if (computed) return;
    
    memset(can_sum, false, sizeof(can_sum));
    can_sum[0] = true;
    
    // For each cube, update can_sum using dynamic programming
    for (unsigned long i = 1; i * i * i <= MAX_VAL; i++) {
        unsigned long cube = i * i * i;
        // Iterate backwards to avoid using the same cube twice
        for (int s = MAX_VAL; s >= (int)cube; s--) {
            if (can_sum[s - (int)cube]) {
                can_sum[s] = true;
            }
        }
    }
    
    computed = true;
}

seq_t a(idx_t n) {
    precompute();
    
    idx_t count = 0;
    for (seq_t current = 1; current <= (seq_t)MAX_VAL; current++) {
        if (!can_sum[(int)current]) {
            count++;
            if (count == n) {
                return current;
            }
        }
    }
    
    return 0;
}
