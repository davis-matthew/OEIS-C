/*
 * A005824  a(n) = 5*a(n-2) - 2*a(n-4), with initial terms 0,1,1,3.
 * https://oeis.org/A005824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) if n is even, else a(n) = 2*a(n-1) +
 *            a(n-2); a(0)=0, a(1)=1, a(2)=1, a(3)=3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 60, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define CACHE_SIZE 20001

static seq_t cache[CACHE_SIZE];
static idx_t max_computed = 0;
static bool initialized = false;

seq_t a(idx_t n) {
    if (n >= CACHE_SIZE) {
        return 0;
    }
    
    if (!initialized) {
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 3;
        max_computed = 3;
        initialized = true;
    }
    
    while (max_computed < n) {
        idx_t i = max_computed + 1;
        if (i % 2 == 0) {
            cache[i] = cache[i-1] + 2 * cache[i-2];
        } else {
            cache[i] = 2 * cache[i-1] + cache[i-2];
        }
        max_computed = i;
    }
    
    return cache[n];
}
