/*
 * A043199  Numbers k such that 2 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043199: numbers k whose base‑9 representation contains
   the adjacent digits 2 and 7 (either "27" or "72") but k‑1 does not. */
#include <stdbool.h>

/* Return true if the base‑9 representation of n contains the
   adjacent digits 2 and 7 (in either order). */
static inline bool has_adjacent_27(seq_t n)
{
    if (n == 0)
        return false;

    int prev_digit = -1;   /* previous (lower place) digit */
    while (n != 0) {
        int d = (int)(n % 9);   /* current least‑significant digit   */
        if (prev_digit == 2 && d == 7)
            return true;
        if (prev_digit == 7 && d == 2)
            return true;
        prev_digit = d;
        n /= 9;
    }
    return false;
}

/* Return the n‑th element of A043199 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* safety fallback – OEIS offset is 1 */

    idx_t count_found = 0;     /* how many valid k have been seen     */
    seq_t k = 1;               /* current candidate integer           */

    while (count_found < n) {
        if (has_adjacent_27(k) && !has_adjacent_27(k - 1)) {
            ++count_found;
            if (count_found == n)
                return k;
        }
        ++k;
    }

    /* Program flow never reaches here, but return 0 to satisfy the compiler. */
    return 0;
}
