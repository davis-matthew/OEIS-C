/*
 * A011688  A binary m-sequence: expansion of reciprocal of x^7+x^5+x^4+x^3+1.
 * https://oeis.org/A011688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-7) XOR a(n-5) XOR a(n-4) XOR a(n-3) for n >= 7, with
 *            initial terms a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0,
 *            a(6)=1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static void init_cache(seq_t *cache) {
    /* initial 7 terms */
    cache[0] = 0; cache[1] = 0; cache[2] = 0; cache[3] = 0;
    cache[4] = 0; cache[5] = 0; cache[6] = 1;
    /* recurrence for n >= 7 up to 126 */
    for (idx_t n = 7; n < 127; ++n) {
        cache[n] = cache[n-7] ^ cache[n-5] ^ cache[n-4] ^ cache[n-3];
    }
}

seq_t a(idx_t n) {
    static seq_t cache[127];
    static bool initialized = false;
    if (!initialized) {
        init_cache(cache);
        initialized = true;
    }
    idx_t idx = n % 127;
    return cache[(size_t)idx];
}
