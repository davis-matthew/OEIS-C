/*
 * A060219  Number of orbits of length n under the full 16-shift (whose
 *          periodic points are counted by A001025).
 * https://oeis.org/A060219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/n) * Σ_{d|n} μ(d) * 16^(n/d)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mu_of.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, safe guard */

    /* Fast path for small integer types (≤ 64‑bit). */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sum = 0;

        for (idx_t d = 1; d <= n; ++d) {
            if (n % d != 0) continue;          /* only divisors of n */
            int mu = mu_of(d);
            if (mu == 0) continue;              /* skip terms with μ=0 */

            idx_t e = n / d;                    /* exponent for 16 */
            unsigned __int128 term = 1;
            for (idx_t i = 0; i < e; ++i) term *= 16;  /* 16^e with 128‑bit */

            if (mu > 0)
                sum += term;
            else
                sum -= term;
        }
        return (seq_t)(sum / n);                /* division is exact */
    }

    /* Generic big‑integer implementation. */
    seq_t sum = 0;

    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0) continue;
        int mu = mu_of(d);
        if (mu == 0) continue;

        idx_t e = n / d;
        seq_t term = big_pow((seq_t)16, (seq_t)e); /* 16^e as big int */

        if (mu > 0)
            sum += term;
        else
            sum -= term;
    }
    return sum / (seq_t)n; /* exact division for big integers */
}
