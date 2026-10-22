/*
 * A005511  a(n) = 1 + a(floor(n/2))*a(ceiling(n/2)) for n > 1, a(1) = 4.
 * https://oeis.org/A005511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 1 + a(floor(n/2))*a(ceiling(n/2)) for n > 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>

#define CACHE_SIZE 65536
static seq_t cache[CACHE_SIZE];
static bool computed[CACHE_SIZE];

seq_t a(idx_t n) {
    if (n < CACHE_SIZE && computed[n]) {
        return cache[n];
    }
    
    seq_t result;
    if (n == 1) {
        result = 4;
    } else {
        idx_t floor_half = n / 2;
        idx_t ceil_half = (n + 1) / 2;
        result = 1 + a(floor_half) * a(ceil_half);
    }
    
    if (n < CACHE_SIZE) {
        cache[n] = result;
        computed[n] = true;
    }
    
    return result;
}
