/*
 * A031027  Write the (n+1)st Fibonacci number in base 7 and juxtapose.
 * https://oeis.org/A031027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth digit (1‑indexed) of the concatenation of Fibonacci
 *            numbers from F₂ onward written in base 7
 * Range:     u8 n <= 24, u16 n <= 78, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Fibonacci numbers: F₁ = 1, F₂ = 1.  We start from F₂. */
    seq_t f_prev = 1;  /* F₁ */
    seq_t f_curr = 1;  /* F₂ */
    idx_t cum = 0;     /* total digits processed so far */

    while (1) {
        /* Length of f_curr in base 7. */
        seq_t tmp = f_curr;
        idx_t len = 0;
        while (tmp > 0) {
            tmp = tmp / 7;
            len++;
        }

        if (cum + len >= n) {               /* n lies within this number */
            idx_t pos = n - cum;            /* 1‑based index inside f_curr */

            /* Extract digits of f_curr in base 7. */
            unsigned char *digits = (unsigned char *)malloc((size_t)len * sizeof(unsigned char));
            seq_t cur = f_curr;
            for (idx_t i = 0; i < len; ++i) {
                idx_t rem = (idx_t)(cur % 7);
                digits[len - 1 - i] = (unsigned char)rem;  /* most significant at index 0 */
                cur = cur / 7;
            }
            seq_t result = digits[pos - 1];
            free(digits);
            return result;
        }

        cum += len;
        /* Move to the next Fibonacci number. */
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;
    }
}
