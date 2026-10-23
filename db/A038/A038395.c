/*
 * A038395  Concatenation of the first n odd numbers in reverse order.
 * https://oeis.org/A038395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of the first n odd numbers in reverse order
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

# include <stdbool.h>

seq_t a(idx_t n)
{
    /* precompute powers of 10 up to 10 digits (sufficient for n <= 10000) */
    static seq_t pow10[11];
    static bool pow10_init = false;
    if (!pow10_init) {
        pow10[0] = 1;
        for (int d = 1; d <= 10; ++d)
            pow10[d] = pow10[d - 1] * 10;
        pow10_init = true;
    }

    seq_t result = 0;

    for (idx_t i = n; i >= 1; --i) {
        idx_t odd_dec = 2 * i - 1;
        seq_t odd = (seq_t)odd_dec;

        /* count decimal digits of odd_dec */
        int digits = 0;
        idx_t tmp = odd_dec;
        do {
            ++digits;
            tmp /= 10;
        } while (tmp > 0);

        result = result * pow10[digits] + odd;
    }

    return result;
}
