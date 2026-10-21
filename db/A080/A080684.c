/*
 * A080684  Number of 13-smooth numbers <= n.
 * https://oeis.org/A080684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of positive integers <= n with all prime factors
 *            <= 13 (including 1). The function enumerates every 13‑smooth
 *            number by back‑tracking over the primes {2,3,5,7,11,13} and
 *            counts them
 * Range:     u8 n <= 275, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   smooth_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/smooth_rec.h"

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) is defined as 0 – harmless for offset 1 */
        return 0;
    seq_t target = (seq_t)n;
    seq_t count  = 0;
    smooth_rec(target, (seq_t)1, 0, &count);
    return count;
}
