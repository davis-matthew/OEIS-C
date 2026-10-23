/*
 * A137359  a(n) = Sum_{k <= n/2 } k*binomial(n-2k, 3k).
 * https://oeis.org/A137359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{k ≤ n/2} k*binomial(n-2k, 3k)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 40, u64 n <= 81, u128 n <= 164,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"     /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t m = n - 2 * k;
        if (m < 3 * k)            /* binom returns 0 when a<b, but check to avoid
                                     unnecessary call */
            continue;
        seq_t b = binom(m, 3 * k); /* binomial coefficient */
        sum += (seq_t)k * b;
    }
    return sum;
}
