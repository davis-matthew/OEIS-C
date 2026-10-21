/*
 * A085942  Write digit reversal of n below n. Then a(n) = the sum of the
 *          product of digits in the same column.
 * https://oeis.org/A085942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ digit_i(n) * digit_{len-1-i}(n) (i = 0..len-1)
 * Range:     u8 n <= 6997, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=0, a(1)=1, a(2)=4, a(3)=9, ...  (OEIS A085942) */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Extract decimal digits of n into rev[], least‑significant first. */
    uint8_t rev[20];  /* max digits for a 64‑bit number */
    size_t len = 0;
    uint64_t tmp = (uint64_t)n;

    if (tmp == 0) {
        rev[0] = 0;
        len = 1;
    } else {
        while (tmp > 0) {
            rev[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        size_t j = len - 1 - i;
        result += (seq_t)rev[i] * rev[j];
    }

    return result;
}
