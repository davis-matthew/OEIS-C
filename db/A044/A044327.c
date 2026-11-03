/*
 * A044327  Numbers n such that string 8,4 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer whose base‑9 representation contains the
   substring “84” while n − 1 does not.  The sequence is OEIS A044327,
   offset 1 (a(1) = 76). */

#include <stdbool.h>

/* Returns true if the base‑9 expansion of x contains the adjacent
   digits 8 followed by 4.  The check is performed while converting
   x to base 9, so it works for any seq_t size. */
static bool has84(seq_t x)
{
    /* An empty representation has no digits. */
    if (x == 0)
        return false;

    /* Scan digits from least significant to most significant.
       Detect the pattern 4 (previous) followed by 8 (current). */
    seq_t prev = 0;          /* previous (less significant) digit */
    bool first = true;       /* flag for the first digit */

    while (x > 0) {
        seq_t digit = x % 9;      /* current least–significant digit */
        x = x / 9;

        if (!first) {
            /* We have seen a previous digit already. */
            if (digit == 8 && prev == 4)
                return true;
        }

        prev = digit;
        first = false;
    }
    return false;
}

/* Returns the n-th value of the sequence (n >= 1).  The function
   increments successive integers until it finds the n‑th one that
   satisfies the definition.  This linear search is fast enough for
   n up to about 10 000, as the density of such numbers is roughly
   1 in 75. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t number = 1;

    for (;; ++number) {
        if (has84(number)) {
            seq_t prev = number - 1;
            if (!has84(prev)) {
                ++count;
                if (count == n)
                    return number;
            }
        }
    }
    /* Unreachable, but keeps the compiler happy. */
    return 0;
}
