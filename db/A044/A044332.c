/*
 * A044332  Numbers k such that the string 0,0 occurs in the base 10
 *          representation of k but not of k-1.
 * https://oeis.org/A044332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 649, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044332: Numbers k such that the string 0,0 occurs in the
   base 10 representation of k but not of k-1.
   The sequence consists of all multiples of 100 whose predecessor
   contains no pair of consecutive zero digits. */

#include <stdbool.h>

/* Test whether x contains two consecutive zeros in its decimal representation. */
static bool has_double_zero(seq_t x) {
    while (x >= 10) {          /* at least two digits left */
        if ((x % 100) == 0)    /* last two digits are 00 */
            return true;
        x /= 10;               /* shift right by one decimal digit */
    }
    return false;
}

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n) {
    if (n == 0) /* offset; not used in normal queries */
        return 0;

    idx_t count = 0;
    seq_t cand = 100;          /* first multiple of 100 */

    while (1) {
        seq_t prev = cand - 1;
        if (!has_double_zero(prev)) {   /* predecessor has no "00" */
            count++;
            if (count == n)
                return cand;
        }
        cand += 100;               /* next multiple of 100 */
    }
}
