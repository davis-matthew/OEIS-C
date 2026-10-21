/*
 * A070359  a(n) = 3^n mod 34.
 * https://oeis.org/A070359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 34
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t period[16];
    static int initialized = 0;

    if (!initialized) {
        seq_t val = 1;                     /* 3^0 mod 34 */
        for (size_t i = 0; i < 16; ++i) {
            period[i] = val;
            val = (val * 3u) % 34u;       /* 3^(i+1) mod 34 */
        }
        initialized = 1;
    }
    /* Use modulus 16 (the period of 3 mod 34) to index the cache. */
    idx_t idx = n % 16u;
    return period[(size_t)idx];
}
