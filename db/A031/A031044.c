/*
 * A031044  a(n)=least k such that base 8 representation of n begins at s(k),
 *          where s=A031035.
 * https://oeis.org/A031044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   octal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least position where the base‑8 representation of n first appears
   in the infinite string obtained by concatenating the base‑8 representations
   of the natural numbers 1,2,3,…   */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/octal_digits.h"

/* Main function requested by the build infrastructure. */
seq_t a(idx_t n)
{
    /* octal representation of n */
    uint8_t pat[32];
    size_t pat_len = octal_digits(n, pat);
    if (pat_len == 0) return (seq_t)0;  /* safety, n>=1 so impossible */

    /* sliding window of the latest pat_len digits of the stream */
    uint8_t win[32];
    size_t win_len = 0;
    idx_t pos = 0;  /* current position in the infinite stream (1‑based) */

    for (idx_t m = 1; ; ++m) {
        /* digits of the current integer m in base‑8 */
        uint8_t digits[32];
        size_t dlen = octal_digits(m, digits);

        for (size_t i = 0; i < dlen; ++i) {
            ++pos;  /* new digit is at position pos */

            if (win_len < pat_len) {
                win[win_len++] = digits[i];
            } else {
                /* shift window left by one and insert new digit */
                memmove(win, win + 1, (pat_len - 1) * sizeof(uint8_t));
                win[pat_len - 1] = digits[i];
            }

            if (win_len == pat_len && memcmp(win, pat, pat_len * sizeof(uint8_t)) == 0) {
                /* pattern found; its starting position is pos - pat_len + 1 */
                return (seq_t)(pos - pat_len + 1);
            }
        }
    }
}
