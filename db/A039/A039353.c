/*
 * A039353  Numbers whose base-8 representation has the same nonzero number of
 *          4's and 7's.
 * https://oeis.org/A039353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039353: Numbers whose base‑8 representation has the same non‑zero
   number of 4’s and 7’s.  Computed by brute‑force enumeration for any n ≥ 1. */

#include <stdbool.h>

/* Return true if x has the same number of non‑zero 4's and 7's in its base‑8
   representation.  The digit counts are stored in local ints.  It works for
   arbitrary‑precision seq_t as well as built‑in integers. */
static bool has_equal_nonzero_47(seq_t x)
{
    int cnt4 = 0, cnt7 = 0;
    while (x > 0)
    {
        seq_t d = x % (seq_t)8;
        if (d == (seq_t)4)
            ++cnt4;
        else if (d == (seq_t)7)
            ++cnt7;
        x /= (seq_t)8;
    }
    return (cnt4 > 0 && cnt4 == cnt7);
}

/* Return the n-th integer (1‑based) whose base‑8 representation contains an
   equal non‑zero number of 4's and 7's.  The function is entirely
   self‑contained – it enumerates candidates from 1 upward until the desired
   count is reached.  It works for any supported seq_t and any idx_t. */
seq_t a(idx_t n)
{
    seq_t candidate = 1;   /* start counting from 1 */
    seq_t result = 0;
    idx_t count = 0;

    while (count < n)
    {
        if (has_equal_nonzero_47(candidate))
        {
            ++count;
            result = candidate;
        }
        ++candidate;
    }

    return result;
}
