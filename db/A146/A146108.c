/*
 * A146108  Bell numbers (A000110) read mod 18.
 * https://oeis.org/A146108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Bell number B_n modulo 18. The sequence has period 39
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t cache[39];

    if (!initialized) {
        /* Compute B_0 … B_38 modulo 18. */
        cache[0] = 1 % 18;          /* B0 = 1 */

        for (idx_t i = 0; i < 38; ++i) {
            seq_t sum = 0;
            uint64_t bin = 1; /* choose(i,0) */

            for (idx_t k = 0; k <= i; ++k) {
                seq_t bmod = bin % 18;              /* binomial coefficient mod 18 */
                seq_t term = (bmod * cache[k]) % 18; /* add B_k * C(i,k) */
                sum += term;
                sum %= 18;

                if (k < i) {
                    /* move to C(i,k+1) = C(i,k) * (i-k)/(k+1) */
                    bin = bin * (i - k) / (k + 1);
                }
            }
            cache[i + 1] = sum % 18;
        }
        initialized = true;
    }

    idx_t idx = n % 39;
    return cache[idx];
}
