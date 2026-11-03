/*
 * A043191  Numbers k such that 1 and 5 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043191: brute‑force search for numbers k such that
   the base‑9 representation of k contains the digits 1 and 5
   next to each other (in either order) while the same property
   does not hold for k‑1.  a(1) = 14. */
#include <stdbool.h>

/* Return true if the base‑9 representation of x contains an
   adjacent pair of digits 1 and 5 in either order. */
static inline bool contains15_or51(seq_t x)
{
    seq_t prev = (seq_t)9;          /* sentinel – not a legal digit */
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)9;     /* least‑significant digit */
        if ((d == (seq_t)1 && prev == (seq_t)5) ||
            (d == (seq_t)5 && prev == (seq_t)1))
            return true;
        prev = d;
        x /= (seq_t)9;
    }
    return false;
}

/* Return the n‑th term of the sequence (n >= 1).  For n == 0
   the function returns 0, as behaviour below the offset does not
   matter. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t k = (seq_t)1;          /* start searching from 1 */
    idx_t count = 0;

    for (;; ++k) {
        if (contains15_or51(k) && !contains15_or51(k - (seq_t)1)) {
            ++count;
            if (count == n)
                return k;
        }
    }
}
