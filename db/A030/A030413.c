/*
 * A030413  Write (n+1)st Fibonacci number in base 4 and juxtapose.
 * https://oeis.org/A030413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th digit of the infinite concatenation of the
 *            base‑4 representations of Fibonacci numbers starting with F₂ = 1
 * Range:     u8 n <= 31, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* target position (1‑based) */
    idx_t target = n;
    idx_t pos = 0;          /* current position in the digit stream */

    /* Fibonacci numbers: start with F₂ and F₃ */
    seq_t f_prev = (seq_t)1;   /* F₂ */
    seq_t f_curr = (seq_t)2;   /* F₃ */

    while (1) {
        /* Convert current Fibonacci number to base–4 digits.
           digits[] holds the digits in reverse order (LSB first). */
        uint8_t digits[8192];   /* ample space for all reasonable n */
        size_t len = 0;
        seq_t tmp = f_prev;

        if (tmp == (seq_t)0) {
            digits[0] = (uint8_t)0;
            len = 1;
        } else {
            while (tmp != (seq_t)0) {
                digits[len++] = (uint8_t)(tmp % (seq_t)4);
                tmp = tmp / (seq_t)4;
            }
        }

        /* Emit digits from most significant to least significant. */
        for (int i = (int)len - 1; i >= 0; --i) {
            ++pos;
            if (pos == target)
                return (seq_t)digits[i];
        }

        /* Advance to the next Fibonacci number. */
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;
    }
}
