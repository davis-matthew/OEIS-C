/*
 * A005510  a(n) = 1 + a(floor(n/2))*a(ceiling(n/2)) for n > 1, a(1) = 3.
 * https://oeis.org/A005510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + a(floor(n/2)) * a(ceiling(n/2)) for n > 1, a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

#define CACHE_SIZE 100000
static seq_t cache[CACHE_SIZE];
static int cache_initialized = 0;

seq_t a(idx_t n) {
    if (n == 1) return 3;
    
    if (!cache_initialized) {
        memset(cache, 0, sizeof(cache));
        cache_initialized = 1;
    }
    
    if (n < CACHE_SIZE && cache[n] != 0) {
        return cache[n];
    }
    
    idx_t half = n / 2;
    idx_t half_ceil = (n + 1) / 2;
    
    seq_t left = a(half);
    seq_t right = a(half_ceil);
    seq_t result = 1 + left * right;
    
    if (n < CACHE_SIZE) {
        cache[n] = result;
    }
    
    return result;
}
