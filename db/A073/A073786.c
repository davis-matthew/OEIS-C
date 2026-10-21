/*
 * A073786  Numbers in base -5.
 * https://oeis.org/A073786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the decimal number obtained by writing n in base –5
 *            (digits 0–4). The function computes the base –5 digits and
 *            builds the corresponding decimal integer
 * Range:     u8 n <= 54, u16 n <= 2604, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* a(0) = 0 */

    /* Array large enough for base‑5 representation of any 64‑bit integer
       (log_5(2^64) ≈ 28 digits).  */
    uint8_t digits[64];
    size_t dlen = 0;

    /* Use a signed intermediate to handle negative quotients.  */
    __int128_t x = n;

    while (x != 0) {
        int r = (int)(x % 5);          /* remainder may be negative   */
        if (r < 0) r += 5;              /* force 0‑4                  */
        digits[dlen++] = (uint8_t)r;    /* store least‑significant digit */

        /* next value of x:  x = (x – r) / –5  (integer division).  */
        x = (x - r) / -5;
    }

    /* Build the final decimal value from most significant to least. */
    seq_t result = 0;
    for (size_t i = dlen; i > 0; --i) {
        result = result * 10 + (seq_t)digits[i - 1];
    }

    return result;
}
