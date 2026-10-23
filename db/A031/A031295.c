/*
 * A031295  Position of n-th 8 in A007376.
 * https://oeis.org/A031295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, base
 * Formula:   a(n) = position of the nth digit 8 in the concatenated sequence
 *            123456789101112…
 * Range:     u8 n <= 22, u16 n <= 5563, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* pos holds the current position in the concatenated string
       (1‑based).  count keeps track of how many 8’s have been seen. */
    seq_t pos = 1;
    idx_t count = 0;

    for (idx_t m = 1; ; ++m) {
        /* Extract the decimal digits of m into a temporary array.
           The array is built in reverse order, then processed
           from the most significant digit to the least. */
        idx_t tmp = m;
        int digits[20];     /* enough for 64‑bit numbers */
        int len = 0;
        do {
            digits[len++] = tmp % 10;
            tmp /= 10;
        } while (tmp > 0);

        /* Iterate over the digits in natural order */
        for (int i = len - 1; i >= 0; --i) {
            if (digits[i] == 8) {
                ++count;
                if (count == n)
                    return (seq_t)pos;   /* found the nth 8 */
            }
            ++pos;       /* advance to the next position */
        }
    }
}
