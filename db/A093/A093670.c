/*
 * A093670  Numbers having a unique representation as ab+ac+bc, with 1 <= a <=
 *          b <= c.
 * https://oeis.org/A093670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   build_sequence_up_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A093670 : Numbers having a unique representation as ab+ac+bc
 *  with 1 <= a <= b <= c.
 *
 *  The sequence is computed by generating all values of
 *  ab+ac+bc up to a limit L, counting how many times each value
 *  occurs.  A value with count==1 is unique.  The list of unique
 *  values is sorted as the final sequence.
 *
 *  On each call to the function we enlarge L until we have at
 *  least the requested n‑th term.  The construction is
 *  deterministic and works for any call order.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_sequence_up_to.h"

/* ---------- private helpers (local to this file) ---------- */

/* ---------- public function required by the build ---------- */

seq_t a(idx_t n)
{
    static idx_t current_limit = 0;    /* last limit used for construction */
    static seq_t *values = NULL;
    static size_t values_len = 0;

    if (n == 0) return (seq_t)0;          /* undefined, but safe */

    while (values_len < (size_t)n)
    {
        idx_t new_limit = current_limit == 0 ? 2000000ULL : current_limit * 2ULL;
        seq_t *new_vals = NULL;
        size_t new_len = 0;

        build_sequence_up_to(new_limit, &new_vals, &new_len);

        if (values) free(values);
        values = new_vals;
        values_len = new_len;
        current_limit = new_limit;
    }

    return values[n - 1];
}
