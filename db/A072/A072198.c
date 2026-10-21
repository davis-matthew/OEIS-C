/*
 * A072198  E12 range of preferred resistor values in electronic engineering.
 * https://oeis.org/A072198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = base[(n-1) mod 12] * 10^floor((n-1)/12) where base =
 *            {10,12,15,18,22,27,33,39,47,56,68,82}
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for one decade of the E12 series */
    static const seq_t base[12] = {
        10, 12, 15, 18, 22, 27,
        33, 39, 47, 56, 68, 82
    };

    /* Convert to zero‑based index: n >= 1 */
    uint64_t idx = (uint64_t)n - 1U;

    /* Position within the decade and number of complete decades before it */
    size_t   pos    = (size_t)(idx % 12U);
    uint64_t decade = idx / 12U;

    /* Start with the base value for this position */
    seq_t val = base[pos];

    /* Multiply by 10 for each full decade (i.e. 10^decade) */
    for (uint64_t i = 0; i < decade; ++i) {
        val *= (seq_t)10;
    }

    return val;
}
