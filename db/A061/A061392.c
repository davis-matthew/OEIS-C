/*
 * A061392  a(n) = a(floor(n/3)) + a(ceiling(n/3)) with a(0) = 0 and a(1) = 1.
 * https://oeis.org/A061392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of nonnegative integers < n having no 1 in their
 *            ternary representation. This is a digit‑DP that runs in O(log_3
 *            n)
 * Range:     u8 n <= 6560, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Extract base‑3 digits of n (low → high). */
    idx_t digits[64];
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = tmp % 3;
        tmp /= 3;
    }

    seq_t result = (seq_t)0;

    /* Process digits from most significant to least. */
    for (size_t pos = 0; pos < len; ++pos) {
        idx_t digit   = digits[len - 1 - pos];
        size_t remaining = len - 1 - pos;

        /* Any allowed digit smaller than 'digit' gives 2^remaining
           possibilities for the remaining positions. */
        if (digit > 0) {
            result += (seq_t)1 << remaining;   /* allowed digits: {0} */
        }

        /* If the current digit is 1, we cannot match n any further. */
        if (digit == 1)
            return result;
        /* If the digit is 0 or 2, we continue to the next position. */
    }

    /* All digits processed without encountering a '1', so n itself
       is NOT counted (we count strictly less than n). */
    return result;
}
