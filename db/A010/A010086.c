/*
 * A010086  Weight distribution of d=3 Hamming code of length 31.
 * https://oeis.org/A010086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0) = 1, a(1) = 0; for n > 1 : a(n) = (binomial(31, n‑1) -
 *            a(n‑1) - (31‑n+2)*a(n‑2)) / n (where binomial is taken from
 *            ../utils/binomial.h)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 15, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The Hamming(31,26) code has weights only up to 31. */
    if (n > 31) return 0;

    static seq_t cache[32] = { [0] = 1, [1] = 0 };
    static bool  computed[32] = { true, true };

    /* Compute all required values up to n. */
    for (idx_t i = 2; i <= n; ++i) {
        if (!computed[i]) {
            seq_t term1 = binomial(31, i - 1);
            seq_t term2 = cache[i - 1];
            seq_t term3 = ((seq_t)(31 - i + 2)) * cache[i - 2];
            seq_t num   = term1 - term2 - term3;
            cache[i]    = num / (seq_t)i;   /* exact division is guaranteed */
            computed[i] = true;
        }
    }
    return cache[n];
}
