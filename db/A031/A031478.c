/*
 * A031478  Numbers whose base-6 representation has one more 0 than 5's.
 * https://oeis.org/A031478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th positive integer whose base‑6 representation
 * contains exactly one more 0 than 5.  The sequence is obtained by
 * enumerating integers in increasing order and testing the digit
 * counts. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true iff x's base‑6 representation has one more 0 digit
 * than 5 digits. */
static bool satisfies(seq_t x)
{
    unsigned zeros = 0, fives = 0;
    while (x != (seq_t)0) {
        seq_t d = x % (seq_t)6;
        if (d == (seq_t)0) ++zeros;
        else if (d == (seq_t)5) ++fives;
        x = x / (seq_t)6;
    }
    return zeros == fives + 1;
}

/* Public interface required by the framework. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;          /* undefined for n < 1 */

    idx_t found = 0;                              /* number of terms seen */
    seq_t current = 1;                            /* integer to test */

    while (found < n) {
        if (satisfies(current)) {
            ++found;
            if (found == n) return current;
        }
        ++current;
    }
    /* unreachable */
    return (seq_t)0;
}
