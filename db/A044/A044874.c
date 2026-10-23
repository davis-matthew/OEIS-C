/*
 * A044874  Numbers having, in base 3, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   base3_balanced
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th positive integer whose base‑3 run‑lengths satisfy
   sum_of_even_runs == sum_of_odd_runs.  The function is evaluated
   independently for each call; it does not rely on any global state. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/base3_balanced.h"
/* seq_t and idx_t are defined by the build system. */

/* Return the n‑th term of OEIS sequence A044874 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) /* Undefined for n<1; return 0 as a safe fallback. */
        return (seq_t)0;

    idx_t found = (idx_t)0;
    seq_t candidate = (seq_t)1;

    while (found < n) {
        if (base3_balanced(candidate)) {
            found++;
            if (found == n)
                return candidate;
        }
        candidate++;
    }

    /* Control reaches here only if a wrap‑around occurs (impossible under
       normal circumstances).  Return the last tested candidate as a safety
       value. */
    return candidate;
}
