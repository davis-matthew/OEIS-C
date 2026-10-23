/*
 * A043319  a(n)=A033013(n)/16.
 * https://oeis.org/A043319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer whose base‑225 representation has
 *            all digits < 15 and no two adjacent digits equal
 * Range:     u8 n <= 28, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against invalid call */

    /* Find the length L of the base‑225 representation of a(n). */
    seq_t cum = 0;                  /* count of numbers with shorter representation */
    seq_t pow14 = 14;               /* 14^L for current L (starting with L = 1) */
    idx_t L = 1;
    while (cum + pow14 < n) {
        cum += pow14;               /* add all numbers of current length */
        pow14 *= 14;                /* next power of 14 */
        ++L;
    }

    /* Index within the block of numbers having length L (0‑based). */
    idx_t posIndex = (idx_t)(n - cum - 1);

    /* Prepare for digit selection.
       block holds 14^(remainingDigits) and will be divided by 14 at each step. */
    seq_t block = pow14;            /* 14^L */
    int prevDigit = -1;             /* previous digit, -1 means none */
    int digits[64];                 /* most significant digit first */
    idx_t remaining = posIndex;

    for (idx_t pos = 0; pos < L; ++pos) {
        block /= 14;                /* 14^(L-pos-1) */

        /* Build list of allowed digits for this position. */
        int allowed[15];
        int cnt = 0;
        if (pos == 0) {             /* most significant digit: 1..14 */
            for (int d = 1; d <= 14; ++d) allowed[cnt++] = d;
        } else {                    /* other positions: 0..14 except prev */
            for (int d = 0; d <= 14; ++d) {
                if (d != prevDigit) allowed[cnt++] = d;
            }
        }

        /* Choose the digit that contains the remaining index. */
        idx_t blockIdx = (idx_t)block;
        idx_t dIdx = remaining / blockIdx;
        int chosen = allowed[dIdx];
        digits[pos] = chosen;

        remaining = remaining % blockIdx;
        prevDigit = chosen;
    }

    /* Convert the digit sequence to an integer (base 225). */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i) {
        value = value * (seq_t)225 + (seq_t)digits[i];
    }
    return value;
}
