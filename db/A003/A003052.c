/*
 * A003052  Self numbers or Colombian numbers (numbers that are not of the
 *          form m + sum of digits of m for any m).
 * https://oeis.org/A003052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A self number cannot be expressed as m + sum_of_digits(m) for
 *            any m. Algorithm: Sieve all numbers of form m + digitsum(m),
 *            remainder are self numbers
 * Range:     u8 n <= 29, u16 n <= 6413, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum_u64.h"

seq_t a(idx_t n) {
    uint64_t max_k = n * 11 + 100;
    
    bool *is_self = (bool *)malloc((size_t)(max_k + 1) * sizeof(bool));
    if (is_self == NULL) {
        return 0;
    }
    
    for (uint64_t i = 0; i <= max_k; i++) {
        is_self[i] = true;
    }
    
    for (uint64_t m = 1; m <= max_k; m++) {
        int ds = digit_sum_u64(m);
        uint64_t k = m + ds;
        if (k <= max_k) {
            is_self[k] = false;
        }
    }
    
    uint64_t count = 0;
    for (uint64_t k = 1; k <= max_k; k++) {
        if (is_self[k]) {
            count++;
            if (count == n) {
                seq_t result = (seq_t)k;
                free(is_self);
                return result;
            }
        }
    }
    
    free(is_self);
    return 0;
}
