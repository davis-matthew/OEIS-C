/*
 * A000382  Restricted permutations.
 * https://oeis.org/A000382
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - 2, with a(4)=6, a(5)=11,
 *            a(6)=20, a(7)=36
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define CACHE_SIZE 10001

static seq_t cache[CACHE_SIZE];
static idx_t max_computed = 0;

seq_t a(idx_t n) {
    if (n < 4) return 0;
    
    if (max_computed == 0) {
        cache[4] = (seq_t)6;
        cache[5] = (seq_t)11;
        cache[6] = (seq_t)20;
        cache[7] = (seq_t)36;
        max_computed = 7;
    }
    
    if (n <= 7) {
        return cache[n];
    }
    
    if (n < CACHE_SIZE) {
        for (idx_t i = max_computed + 1; i <= n; i++) {
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3] - 2;
        }
        if (n > max_computed) max_computed = n;
        return cache[n];
    }
    
    seq_t a4 = cache[4], a5 = cache[5], a6 = cache[6];
    for (idx_t i = 8; i <= n; i++) {
        seq_t a_next = a4 + a5 + a6 - 2;
        a4 = a5;
        a5 = a6;
        a6 = a_next;
    }
    return a6;
}
