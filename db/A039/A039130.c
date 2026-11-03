/*
 * A039130  Numbers whose base-10 representation has the same number of 2's
 *          and 5's.
 * https://oeis.org/A039130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the n-th nonnegative integer whose decimal representation
   contains an equal number of 2’s and 5’s. Calculation is performed by brute‑force
   enumeration. */
#include <stdbool.h>

/* Return true if cand has the same number of digits 2 and 5. */
static inline bool equal25(idx_t cand)
{
    seq_t v = (seq_t)cand;
    unsigned cnt2 = 0, cnt5 = 0;
    seq_t ten  = (seq_t)10;
    seq_t two  = (seq_t)2;
    seq_t five = (seq_t)5;

    /* For cand == 0 the loop is skipped; cnt2 and cnt5 remain 0, which matches
       the definition a(1) = 0. */
    while (v > (seq_t)0) {
        seq_t d = v % ten;
        if (d == two)      ++cnt2;
        else if (d == five) ++cnt5;
        v = v / ten;
    }
    return cnt2 == cnt5;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t cand = 0; ; ++cand) {
        if (equal25(cand)) {
            ++count;
            if (count == n) {
                return (seq_t)cand;
            }
        }
    }
}
