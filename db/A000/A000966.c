/*
 * A000966  n! never ends in this many 0's.
 * https://oeis.org/A000966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2168, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n-th positive integer that never appears as the
 * number of trailing zeros of any factorial m!.
 * The number of trailing zeros in m! is floor(m/5) + floor(m/25) + floor(m/125) + ...
 * This sequence lists the values skipped by this function.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t value_max = (seq_t)n * 8 + 200;
    seq_t m_max = value_max * 5;
    
    uint64_t vm_64 = (uint64_t)value_max;
    bool *appears = (bool *)calloc(vm_64 + 1, sizeof(bool));
    
    // Mark all trailing_zeros values that appear
    for (seq_t m = 0; m <= m_max; m++) {
        seq_t tz = 0;
        seq_t power_of_5 = 5;
        while (power_of_5 <= m) {
            tz += m / power_of_5;
            power_of_5 *= 5;
        }
        
        if (tz <= value_max) {
            appears[(uint64_t)tz] = true;
        }
    }
    
    // Count and find the n-th gap
    seq_t gap_count = 0;
    for (seq_t val = 1; val <= value_max; val++) {
        if (!appears[(uint64_t)val]) {
            gap_count++;
            if (gap_count == n) {
                free(appears);
                return val;
            }
        }
    }
    
    free(appears);
    return 0;
}
