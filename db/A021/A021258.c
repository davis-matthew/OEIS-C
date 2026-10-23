/*
 * A021258  Decimal expansion of 1/254.
 * https://oeis.org/A021258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/254 (0‑indexed)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 1/254 = 0.003937007874015748…  (period 42) */
    /* Compute the n-th decimal digit by ordinary long division. */
    uint64_t r = 1;               /* current remainder (0 < r < 254)   */
    idx_t i;

    for (i = 0; i <= n; ++i) {
        uint64_t d = (r * 10u) / 254u; /* next decimal digit (0…9) */
        if (i == n)
            return (seq_t)d;          /* result fits in any seq_t */
        r = (r * 10u) % 254u;         /* update remainder   */
    }
    return 0; /* unreachable; keeps compiler happy */
}
