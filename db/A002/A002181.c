/*
 * A002181  Least number k such that phi(k) = m, where m runs through the
 *          values (A002202) taken by phi.
 * https://oeis.org/A002181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Inverse of Euler totient function for values in range of phi.
 *            a(n) = smallest k such that phi(k) = m, where m is the n-th
 *            value in A002202
 * Range:     u8 n <= 63, u16 n <= 7380, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/phi_u64.h"

seq_t a(idx_t n) {
    uint64_t n64 = (uint64_t)n;
    
    if (n64 == 1) return (seq_t)1;
    
    uint64_t search_limit = 32 * n64;
    if (search_limit < 100) search_limit = 100;
    if (search_limit > 1000000) search_limit = 1000000;
    
    bool *in_range = (bool *)calloc(search_limit + 1, sizeof(bool));
    if (!in_range) return 0;
    
    for (uint64_t k = 1; k <= 2 * search_limit; k++) {
        uint64_t pk = phi_u64(k);
        if (pk <= search_limit) {
            in_range[pk] = true;
        }
    }
    
    int count = 0;
    uint64_t target_m = 0;
    for (uint64_t m = 1; m <= search_limit; m++) {
        if (in_range[m]) {
            count++;
            if (count == (int)n64) {
                target_m = m;
                break;
            }
        }
    }
    
    free(in_range);
    
    if (target_m == 0) return 0;
    
    for (uint64_t k = 1; k <= 2 * target_m + 2; k++) {
        if (phi_u64(k) == target_m) {
            return (seq_t)k;
        }
    }
    
    return 0;
}
