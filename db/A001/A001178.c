/*
 * A001178  Fibonacci frequency of n.
 * https://oeis.org/A001178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least k such that iterating f(x) = A001175(x)
 *            reaches a fixed point
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t pisano_cache[100000];

static uint64_t pisano_period(uint64_t n) {
    if (n == 1) return 1;
    
    if (n < 100000 && pisano_cache[n] != 0) {
        return pisano_cache[n];
    }
    
    uint64_t a = 0, b = 1;
    
    for (uint64_t i = 1; i <= 6 * n + 100; i++) {
        uint64_t c = (a + b) % n;
        a = b;
        b = c;
        
        if (a == 0 && b == 1) {
            if (n < 100000) {
                pisano_cache[n] = i;
            }
            return i;
        }
    }
    
    return n;
}

seq_t a(idx_t n) {
    uint64_t val = n;
    seq_t count = 0;
    
    while (count < 1000) {
        uint64_t next = pisano_period(val);
        if (next == val) {
            return count;
        }
        val = next;
        count++;
    }
    
    return count;
}
