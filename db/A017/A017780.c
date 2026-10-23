/*
 * A017780  Binomial coefficients C(64,n).
 * https://oeis.org/A017780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(64, n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t cache[65];
    static bool init = false;

    if (!init) {
        cache[0] = 1;
        for (idx_t k = 1; k <= 64; ++k) {
            /* Compute cache[k] = cache[k-1] * (64-k+1) / k
               using 128‑bit arithmetic to avoid overflow. */
            unsigned __int128 prod = (unsigned __int128)cache[k-1] * (unsigned long long)(64 - k + 1);
            cache[k] = (seq_t)(prod / (unsigned long long)k);
        }
        init = true;
    }

    return (n <= 64) ? cache[n] : (seq_t)0;
}
