/*
 * A044283  Numbers n such that string 3,5 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044283: numbers n such that the base‑9 representation of n
   contains the substring "35" while the representation of n‑1 does not. */

#include <stdbool.h>

/* Return true if v in base 9 contains the digits 3 followed by 5
   (i.e., a 3 in a more significant position than a 5). */
static bool has35(seq_t v)
{
    int prev = -1;                     /* previous (less significant) digit */
    while (v) {
        int cur = (int)(v % 9);        /* least significant digit */
        if (cur == 3 && prev == 5)    /* 3 more significant than 5 found */
            return true;
        prev = cur;
        v /= 9;
    }
    return false;
}

/* Return the n-th term (offset 1) of the sequence. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cand = 0;                    /* candidate number */
    while (1) {
        cand++;                        /* next integer to test */
        if (has35(cand) && !has35(cand - 1)) {
            found++;
            if (found == n)
                return cand;
        }
    }
}
