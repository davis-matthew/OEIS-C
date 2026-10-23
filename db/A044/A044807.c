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
 * Helpers:   contains94
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the decimal representation of k
   contains the substring "94" but k+1 does not.  A brute‑force
   incremental search is sufficient for all n up to the problem
   limits. */
#include <stdbool.h>
#include "../utils/contains94.h"

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
