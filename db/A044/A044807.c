/*
 * A044807  Numbers k such that string 9,4 occurs in the base 10
 *          representation of k but not of k+1.
 * https://oeis.org/A044807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the decimal representation of k
   contains the substring "94" but k+1 does not.  A brute‑force
   incremental search is sufficient for all n up to the problem
   limits. */
#include <stdbool.h>

/* Helper: does the decimal representation of x contain "94"? */
static bool contains94(seq_t x)
{
    if (x == 0) return false;
    int prev = -1;                /* previous (less significant) digit */
    while (x > 0) {
        int curr = (int)(x % 10); /* current least significant digit */
        if (prev == 4 && curr == 9) return true;  /* found "94" */
        prev = curr;
        x /= 10;
    }
    return false;
}

/* Return the n-th term (with offset 1) of A044807. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 1 */
    seq_t count = 0;
    seq_t candidate = 94;                 /* first possible number */
    while (true) {
        if (contains94(candidate) && !contains94(candidate + 1)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
