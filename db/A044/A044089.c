/*
 * A044089  Numbers k such that string 2,2 occurs in the base 5 representation
 *          of k but not of k-1.
 * https://oeis.org/A044089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2357, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044089:
   a(n) = the n-th integer k such that the base‑5 representation of k
   contains the substring "22" while the base‑5 representation of k‑1
   does not contain "22".  Off‑set is 1 (a(1) = 12). */
#include <stdbool.h>

/* true if base‑5 representation of x contains the consecutive digits 2,2 */
static bool has_22(seq_t x)
{
    seq_t prev = 0;
    bool first = true;
    while (x > 0) {
        seq_t digit = x % 5;          /* last base‑5 digit */
        if (!first && digit == 2 && prev == 2)
            return true;
        prev = digit;
        first = false;
        x /= 5;
    }
    return false;
}

/* external linkage: compute a(n) */
seq_t a(idx_t n)
{
    idx_t  found = 0;
    seq_t  k     = 1;                /* start from 1 */

    while (found < n) {
        if (has_22(k)) {
            seq_t km1 = k - 1;
            if (!has_22(km1)) {
                ++found;
                if (found == n)
                    return k;
            }
        }
        ++k;
    }
    /* should never reach here */
    return 0;
}
