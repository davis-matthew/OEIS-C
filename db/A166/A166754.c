/*
 * A166754  a(n) = 4*A061547(n+2) - 3*A166753(n).
 * https://oeis.org/A166754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(n+3) + (-1)^n - (4*n+7))/2 This implementation works
 *            for any seq_t supported by the build. For 64‑bit seq_t it uses
 *            unsigned __int128 intermediates so that no overflow occurs on
 *            the 2^(n+3) term. For arbitrary‑precision seq_t it falls back to
 *            the provided big_pow routine
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 64‑bit seq_t: use __int128 intermediate */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow2 = 1;
        /* compute 2^(n+3) in at most 127 shifts (enough for n ≤ 62) */
        for (idx_t i = 0; i < n + 3 && i < 127; ++i)
            pow2 <<= 1;

        unsigned __int128 term = (unsigned __int128)(4 * n + 7);
        unsigned __int128 numerator;

        if ((n & 1) == 0)
            numerator = pow2 + 1 - term;   /* n even  → +1 */
        else
            numerator = pow2 - 1 - term;   /* n odd   → -1 */

        return (seq_t)(numerator / 2);
    }

    /* arbitrary‑precision seq_t: rely on big_pow */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n + 3));
    seq_t term = (seq_t)(4 * n + 7);
    seq_t numerator;

    if ((n & 1) == 0)
        numerator = pow2 + 1 - term;   /* n even  → +1 */
    else
        numerator = pow2 - 1 - term;   /* n odd   → -1 */

    return numerator / 2;
}
