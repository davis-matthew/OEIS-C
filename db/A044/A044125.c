/*
 * A044125  Numbers k such that string 3,5 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest k such that the base‑6 representation of k
 *            contains the substring "35" while the base‑6 representation of
 *            k-1 does not. Offset: 1 (a(1) = 23)
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether a number contains the pattern 3,5 in its base‑6 expansion. */
static bool contains_35(seq_t x)
{
    if (x == 0)
        return false;
    int prev = -1;            /* digit of lower significance */
    while (x != 0) {
        int digit = (int)(x % 6);
        if (prev != -1 && digit == 3 && prev == 5)
            return true;
        prev = digit;
        x /= 6;
    }
    return false;
}

/* Main sequence function. Returns the nth member of A044125. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for 0; return 0 as a safe fallback */
        return 0;

    seq_t k = 1;           /* start scanning from 1 */
    idx_t count = 0;       /* how many suitable numbers have been found */

    while (1) {
        seq_t prev = k - 1;          /* k-1 is safe because k never becomes 0 */
        if (contains_35(k) && !contains_35(prev)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
