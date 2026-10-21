/*
 * A073787  Numbers in base -6.
 * https://oeis.org/A073787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = representation of n in base –6 using digits 0…5, read as
 *            a decimal integer
 * Range:     u8 n <= 77, u16 n <= 6665, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the zero case explicitly. */
    if (n == 0)
        return (seq_t)0;

    /* Digits of the base –6 representation (least‑significant first). */
    unsigned digits[128];
    size_t len = 0;

    /* Use signed arithmetic so that the negative divisor works. */
    int64_t cur = (int64_t)n;

    while (cur != 0) {
        int64_t r = cur % (int64_t)(-6);  /* remainder, may be negative   */
        int64_t q = cur / (int64_t)(-6);  /* quotient                    */
        if (r < 0) {                     /* adjust into 0…5              */
            r += 6;
            q += 1;
        }
        digits[len++] = (unsigned)(int)r;
        cur = q;
    }

    /* Assemble the decimal integer from the most‑significant digit. */
    seq_t res = 0;
    for (int64_t i = (int64_t)len - 1; i >= 0; --i) {
        res = res * (seq_t)10 + (seq_t)digits[i];
    }

    return res;
}
