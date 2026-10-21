/*
 * A093092  "Fibonacci in digits - up and down": start with a(1)=1, a(2)=1;
 *          repeatedly adjoin either the sum of the two previous terms (if
 *          that sum happens to be even) or else adjoin digits of the sum of
 *          previous two terms (if that sum happens to be odd).
 * https://oeis.org/A093092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Fibonacci‑like with digit expansion. a(1)=1, a(2)=1; for
 *            each step, let s = a(k)+a(k+1). If s is even, append s as the
 *            next term. If s is odd, append the decimal digits of s (most
 *            significant digit first). The next step always uses the pair
 *            starting with a(k+1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100, u128 none,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Compute a(n).  Returns 0 if n == 0 (undefined). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    /* Work with native integer types for the loop counter. */
    size_t target = (size_t)n;

    /* Allocate space for at most target terms. */
    seq_t *seq = (seq_t *)malloc(target * sizeof(seq_t));
    if (!seq) return (seq_t)0;            /* allocation failure */

    seq[0] = 1;              /* a(1) */
    seq[1] = 1;              /* a(2) */
    size_t cur_len = 2;      /* current number of terms in seq */
    size_t p = 0;            /* index of the first term of the pair to sum */

    /* Temporary buffer for digits of an odd sum. */
    uint8_t digit_buf[40];   /* 128‑bit integer < 39 digits, so 40 is safe */

    while (cur_len < target) {
        seq_t sum = seq[p] + seq[p + 1];

        /* judge parity of the sum */
        seq_t two = 2;
        if (sum % two == 0) {          /* even sum */
            seq[cur_len++] = sum;
        } else {                       /* odd sum – split into digits */
            size_t dcnt = 0;
            seq_t tmp = sum;
            if (tmp == 0) {
                digit_buf[dcnt++] = 0;
            } else {
                while (tmp != 0) {
                    digit_buf[dcnt++] = (uint8_t)(tmp % 10);
                    tmp /= 10;
                }
            }
            for (size_t i = dcnt; i > 0 && cur_len < target; --i) {
                seq[cur_len++] = digit_buf[i - 1];
            }
        }

        ++p; /* advance to next pair */
    }

    seq_t result = seq[target - 1];
    free(seq);
    return result;
}
