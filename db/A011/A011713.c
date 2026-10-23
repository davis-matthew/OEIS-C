/*
 * A011713  A binary m-sequence: expansion of reciprocal of x^8+x^6+x^5+x+1.
 * https://oeis.org/A011713
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

/* Recurrence: a(n) = a(n-1) ^ a(n-5) ^ a(n-6) ^ a(n-8) for n ≥ 8
   Initial terms: a(0) = 0 … a(6) = 0, a(7) = 1   (the sequence is 255‑periodic) */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache of one period (255 bits) */
    static seq_t cache[255];
    static bool built = false;

    if (!built) {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 0;
        cache[5] = 0;
        cache[6] = 0;
        cache[7] = 1;
        for (size_t i = 8; i < 255; ++i) {
            cache[i] = cache[i-1] ^ cache[i-5] ^ cache[i-6] ^ cache[i-8];
        }
        built = true;
    }

    size_t idx = (size_t)(n % 255);
    return cache[idx];
}
