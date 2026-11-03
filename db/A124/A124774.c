/*
 * A124774  Multinomial coefficients for compositions in standard order.
 * https://oeis.org/A124774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = (∑ b(i))! / (∏ b(i)!) where b = composition number n in
 *            standard order (A066099)
 * Range:     u8 n <= 46, u16 n <= 350, u32 n <= 8190, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 86 terms through n = 9428 (2026-11-01)
 * Helpers:   binomial, ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ilog2.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    /* Handle the empty composition for n = 0. */
    if (n == 0u)
        return seq_one();

    /* Find the sum m of the composition.
       Compositions of sum m start at index 2^(m-1).  The condition
       2^(m-1) <= n < 2^m implies m = floor_log2(n) + 1. */
    idx_t m = (idx_t)ilog2(n) + (idx_t)1u;

    /* r is the 0‑based offset inside the block of compositions of sum m. */
    idx_t r = n - ((idx_t)1u << (m - 1u));

    idx_t remaining = m;
    idx_t pos = 0;
    idx_t parts[64] = {0};

    /* Unrank the composition in descending lexicographic order. */
    while (remaining > 0)
    {
        for (idx_t b = remaining; b >= 1; --b)
        {
            idx_t rem_minus_b = remaining - b;
            idx_t cnt = (rem_minus_b == 0u) ?
                        (idx_t)1u :
                        ((idx_t)1u << (rem_minus_b - 1u));

            if (r < cnt)
            {
                parts[pos++] = b;
                remaining -= b;
                break;
            }
            else
            {
                r -= cnt;
            }
        }
    }

    /* Compute the multinomial coefficient:
       m! / Π b(i)! = Π_i C(remaining, b_i). */
    seq_t result = seq_one();
    remaining = m;
    for (idx_t i = 0u; i < pos; ++i)
    {
        seq_t bin = binomial(remaining, parts[i]);
        result = result * bin;
        remaining -= parts[i];
    }

    return result;
}
