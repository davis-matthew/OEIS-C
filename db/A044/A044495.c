/*
 * A044495  Numbers k such that string 2,0 occurs in the base 6 representation
 *          of k but not of k+1.
 * https://oeis.org/A044495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence from OEIS A044495: numbers k such that "20" occurs in the base‑6 representation of k but
   does not occur in the representation of k+1.
   The implementation performs a brute‑force search from 1 upward, testing each candidate. */
/* #include <stdbool.h> for the bool type used below. */
#include <stdbool.h>

static bool has_20_base6(seq_t x)
{
    /* Test whether the base‑6 string of x contains the substring '20'.
       We scan the digits from least significant upwards so that a
       pattern ...2 0... is detected as cur==2 && prev==0. */
    if (x < 6)          /* one digit: impossible to contain '20' */
        return false;

    seq_t prev = x % 6;
    x /= 6;
    while (x > 0) {
        seq_t cur = x % 6;       /* a more significant digit */
        if (cur == 2 && prev == 0)
            return true;
        prev = cur;
        x /= 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Find the n‑th integer k (n≥1) satisfying the condition. */
    idx_t  found = 0;
    seq_t  k     = 0;          /* starting from 0, will increment before test */

    for (;; ++k) {
        if (has_20_base6(k)) {                /* k contains '20' */
            /* Only proceed if k+1 does NOT contain '20'; */
            if (!has_20_base6((seq_t)(k + 1))) {
                ++found;
                if (found == n)
                    return k;
            }
        }
    }
}
