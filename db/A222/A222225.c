/*
 * A222225  In the number n, replace all (decimal) digits '1' with '8' and
 *          vice versa.
 * https://oeis.org/A222225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A222225: replace each decimal digit 1 with 8 and 8 with 1. */
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Special case: 0 maps to 0. */
    if ((seq_t)n == (seq_t)0)
        return (seq_t)0;

    seq_t x      = (seq_t)n;
    const seq_t TEN   = (seq_t)10;
    const seq_t ONE   = (seq_t)1;
    const seq_t EIGHT = (seq_t)8;

    /* Temporary storage for the mapped digits in reverse order.
       n is an idx_t, so it has at most 20 decimal digits; 32 is safe. */
    seq_t tmp[32];
    size_t len = 0;

    /* Extract digits, map them, and store reversed. */
    while (x != (seq_t)0) {
        seq_t d = x % TEN;
        seq_t m;
        if (d == ONE)
            m = EIGHT;
        else if (d == EIGHT)
            m = ONE;
        else
            m = d;
        tmp[len++] = m;
        x = x / TEN;
    }

    /* Reconstruct the number from the most significant mapped digit. */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < len; ++i) {
        result = result * TEN + tmp[len - 1 - i];
    }

    return result;
}
