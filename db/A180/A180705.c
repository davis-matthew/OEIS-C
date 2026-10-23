/*
 * A180705  Smallest power of 19 that begins with n.
 * https://oeis.org/A180705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 4, u128 n <= 13,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest power of 19 that begins with the decimal representation of n.
 * For each n ≥ 1, returns the smallest non‑negative integer k such that
 * the decimal expansion of 19ⁿ starts with the digits of n.
 */
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Target value (the digits we have to match). */
    seq_t target = (seq_t)n;
    /* Number of decimal digits of the target. */
    size_t target_len = digit_len(target);

    /* Start with 19⁰ = 1 and iteratively multiply by 19. */
    seq_t value = (seq_t)1;
    for (;;)
    {
        /* Only powers with enough digits can match. */
        size_t len = digit_len(value);
        if (len >= target_len)
        {
            /* Determine the divisor 10^(len‑target_len). */
            idx_t shift = (idx_t)(len - target_len);
            seq_t divisor = pow10(shift);
            /* Most significant target_len digits of the current power. */
            seq_t prefix = value / divisor;
            if (prefix == target)
                return value;   /* found the smallest matching power */
        }
        /* Advance to the next power of 19. */
        value = value * (seq_t)19;
    }
}
