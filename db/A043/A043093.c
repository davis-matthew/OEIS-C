/*
 * A043093  Every string of 2 consecutive base 7 digits contains exactly 2
 *          distinct numbers.
 * https://oeis.org/A043093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth non‑negative integer whose base‑7 representation
   contains no two consecutive identical digits (OEIS A043093). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 0 through 6 are allowed. */
    if (n < 7)
        return (seq_t)n;

    /* offset after the 1‑digit numbers */
    idx_t rem = n - 7;

    /* Find the length (number of base‑7 digits) of the desired number. */
    idx_t pow6 = 36;                  /* 6^2  */
    int    len  = 2;                  /* current length being tested */
    while (rem >= pow6) {
        rem -= pow6;
        ++len;
        pow6 *= 6;                    /* now 6^(len+1) */
    }

    /* Pre‑compute powers of 6 up to the needed length. */
    idx_t pow6a[30];
    pow6a[0] = 1;
    for (int i = 1; i < 30; ++i)
        pow6a[i] = pow6a[i-1] * 6;

    /* Build the base‑7 number digit by digit. */
    int   prev  = -1;
    seq_t value = 0;
    for (int pos = 0; pos < len; ++pos) {
        idx_t block = pow6a[len - 1 - pos];          /* combinations after this digit */
        int   start = (pos == 0) ? 1 : 0;            /* leading digit cannot be 0 */
        for (int d = start; d <= 6; ++d) {
            if (pos > 0 && d == prev)
                continue;                            /* skip equal consecutive digits */

            if (rem < block) {                       /* choose this digit */
                value = value * (seq_t)7 + (seq_t)d;
                prev  = d;
                break;
            }
            rem -= block;                            /* skip whole block */
        }
    }
    return value;
}
