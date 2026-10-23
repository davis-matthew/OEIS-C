/*
 * A048522  Terms of Binary Gleichniszahlen-Reihe (BGR) sequence A045998
 *          converted into decimal (Look and Say Sequence, mod 2, read in
 *          binary and converted to decimal).
 * https://oeis.org/A048522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 37, u64 n <= 77, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS A048522 “Binary Gleichniszahlen‑Reihe”:
   start with the binary string “1”.  For each step, run‑length encode
   the current string; for each run output the parity of its length
   (0 if even, 1 if odd) followed by the run bit itself.  The
   resulting binary string is then interpreted as a binary number,
   yielding a(n).  a(0) = 1.  The function handles arbitrary‑precision
   seq_t, and is safe for any idx_t n ≥ 0.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case: the very first term is 1. */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Start with the initial bit string "1". */
    char *curr = (char *)malloc(2);       /* 1 + NUL */
    curr[0] = '1';
    curr[1] = '\0';
    size_t curr_len = 1;

    /* Generate the sequence up to the desired index. */
    for (idx_t step = 1; step <= n; ++step) {
        /* Allocate space for the next string: at most twice the
           current length, plus a NUL byte. */
        size_t next_cap = curr_len * 2 + 1;
        char *next = (char *)malloc(next_cap);

        size_t pos = 0;
        for (size_t i = 0; i < curr_len; ) {
            char bit = curr[i];
            int parity = 1;        /* parity of the run length */
            size_t j = i + 1;
            while (j < curr_len && curr[j] == bit) {
                parity ^= 1;       /* toggle for each additional bit */
                ++j;
            }
            next[pos++] = '0' + parity;   /* output parity bit */
            next[pos++] = bit;            /* output the bit itself */
            i = j;
        }
        next[pos] = '\0';

        free(curr);
        curr = next;
        curr_len = pos;
    }

    /* Convert the final binary string to a decimal value. */
    seq_t value = (seq_t)0;
    for (size_t i = 0; i < curr_len; ++i)
        value = (value << 1) | (seq_t)(curr[i] - '0');

    free(curr);
    return value;
}
