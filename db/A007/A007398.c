/*
 * A007398  Add 7, then reverse digits.
 * https://oeis.org/A007398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* a(n) = reverse_digits(a(n-1) + 7), a(0) = 0; periodic with period 63 for n >= 1 */

static seq_t reverse_digits(seq_t x) {
    seq_t result = 0;
    while (x > 0) {
        result = result * 10 + (x % 10);
        x /= 10;
    }
    return result;
}

seq_t a(idx_t n) {
    static seq_t cache[64] = {0};
    static bool initialized = false;
    
    if (!initialized) {
        cache[0] = 0;
        for (idx_t i = 1; i < 64; i++) {
            cache[i] = reverse_digits(cache[i-1] + 7);
        }
        initialized = true;
    }
    
    if (n < 64) {
        return cache[n];
    } else {
        idx_t index = 1 + ((n - 1) % 63);
        return cache[index];
    }
}
