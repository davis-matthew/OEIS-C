/*
 * A011721  A binary m-sequence: expansion of reciprocal of x^8+x^6+x^5+x^3+1.
 * https://oeis.org/A011721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8) ^ a(n-6) ^ a(n-5) ^ a(n-3), period 255. Initial
 *            terms: a(0)=0,...,a(6)=0, a(7)=1. The sequence repeats with
 *            period 255
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t cache[255];
    static bool init = false;

    if (!init) {
        /* All elements are zero-initialised; set the single 1. */
        cache[7] = (seq_t)1;
        for (idx_t i = 8; i < 255; ++i) {
            cache[i] =
                cache[i - 8] ^ cache[i - 6] ^ cache[i - 5] ^ cache[i - 3];
        }
        init = true;
    }

    return cache[n % 255];
}
