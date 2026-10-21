/*
 * A217579  a(1) = 1; for n > 1, a(n) = max(d*lpf(d) : d|n, d > 1), where lpf
 *          is the least prime factor function (A020639).
 * https://oeis.org/A217579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = max(d * lpf(d) : d|n, d > 1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   lpf(d) is the least prime factor of d. */

#include <stdint.h>
#include "../utils/_lpf.h"

/* Main sequence function.  n is 1‑based; a(1) = 1. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    seq_t best = 0;

    /* Enumerate divisors of n.  We avoid sqrt() to keep all
       arithmetic purely integer. */
    for (idx_t d = 1; (__int128)d * d <= n; ++d)
    {
        if (n % d != 0) continue;

        idx_t d2 = n / d;  /* complementary divisor */

        if (d > 1) {
            uint64_t l = _lpf((uint64_t)d);
            seq_t v = (seq_t)d * (seq_t)l;
            if (v > best) best = v;
        }

        if (d2 != d && d2 > 1) {
            uint64_t l = _lpf((uint64_t)d2);
            seq_t v = (seq_t)d2 * (seq_t)l;
            if (v > best) best = v;
        }
    }

    return best;
}
