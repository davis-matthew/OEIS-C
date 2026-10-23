/*
 * A028807  Nonsquares mod 94.
 * https://oeis.org/A028807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer that is NOT a quadratic residue modulo 94
   (i.e., there is no integer x with x^2 ≡ a(n) (mod 94)).  The routine
   builds a Boolean table of the 94 residues that are squares modulo 94 and
   scans successive integers until the nth non‑residue is found.  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Table of residues that are squares modulo 94.  */
    static bool is_square[94];
    static bool initialized = false;

    if (!initialized) {
        /* Zero‑initialize is_square is implicit, but explicit assignment
           makes the intent clear and keeps diagnostics quiet. */
        for (idx_t i = 0; i < 94; ++i)
            is_square[i] = false;

        /* Squares modulo 94 repeat with period 188.  Compute residues
           for x = 0 .. 187 and mark them. */
        for (idx_t x = 0; x < 188; ++x) {
            seq_t r = ((seq_t)x * (seq_t)x) % 94;
            is_square[(idx_t)r] = true;
        }
        initialized = true;
    }

    seq_t cur = (seq_t)1;     /* Candidate integer. */
    idx_t count = 0;          /* Number of nonresidues seen so far. */

    for (;;) {
        idx_t r = (idx_t)(cur % 94);
        if (!is_square[r]) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
