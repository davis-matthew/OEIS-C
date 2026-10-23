/*
 * A011717  A binary m-sequence: expansion of reciprocal of x^8+x^5+x^3+x^2+1.
 * https://oeis.org/A011717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-8) xor a(n-5) xor a(n-3) xor a(n-2)
 * Initial values: a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=0,a(6)=0,a(7)=1
 * (period 255)
 */
#include <stddef.h>
#include <stdbool.h>

#define PERIOD 255

seq_t a(idx_t n)
{
    static seq_t cache[PERIOD] = {0};
    static bool initialized = false;
    if (!initialized) {
        // Set known initial values
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)0;
        cache[5] = (seq_t)0;
        cache[6] = (seq_t)0;
        cache[7] = (seq_t)1;
        // Compute the rest of the period
        for (idx_t i = 8; i < PERIOD; ++i) {
            cache[i] = cache[i-8] ^ cache[i-5] ^ cache[i-3] ^ cache[i-2];
        }
        initialized = true;
    }
    return cache[n % PERIOD];
}
