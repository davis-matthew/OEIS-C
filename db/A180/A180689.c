/*
 * A180689  Smallest power of 11 that begins with n.
 * https://oeis.org/A180689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   smallest power of 11 that begins with the decimal representation
 *            of n
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 14,
 *            big any n
 * Helpers:   digit_len, digits_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits_of.h"
#include "../utils/digit_len.h"

/* Return the smallest power of 11 that starts with the integer n (n >= 1). */
seq_t a(idx_t n)
{
    /* Handle the trivial case n == 1 (11^0). */
    if (n == 1) return 1;

    /* Number of decimal digits required for the prefix. */
    idx_t d_n = digits_of(n);

    seq_t power = 1;              /* Current power of 11.       */
    idx_t len   = 1;              /* Decimal length of power.   */

    /* Advance until the power has at least d_n digits. */
    while (len < d_n) {
        power *= (seq_t)11;
        len   = digit_len(power);
    }

    /* Divisor 10^(len - d_n) used to extract the leading d_n digits. */
    seq_t divisor = 1;
    for (idx_t i = 0; i < len - d_n; ++i) divisor *= (seq_t)10;

    /* Search for the minimal exponent. */
    while (1) {
        seq_t leading = power / divisor;
        if (leading == (seq_t)n) return power;   /* Match found. */

        /* Move to the next power of 11. */
        power *= (seq_t)11;
        idx_t new_len = digit_len(power);

        /* If the number of digits increased, adjust the divisor. */
        if (new_len > len) {
            for (idx_t i = 0; i < new_len - len; ++i) divisor *= (seq_t)10;
            len = new_len;
        }
    }
}
