/*
 * A074940  Numbers having at least one 2 in their ternary representation.
 * https://oeis.org/A074940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 217, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 8972 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074940: numbers having at least one digit 2 in their ternary representation. */
/* The function scans integers in order and counts those whose base‑3 representation
   contains the digit 2. The n‑th such integer is returned.  Complexity is O(n). */

#include <stdbool.h>

/* Helper: true if v has a ternary digit equal to 2. */
static inline bool has_two_in_base3(seq_t v)
{
    seq_t three = (seq_t)3;
    for(; v != (seq_t)0; v = v / three)
    {
        if ((v % three) == (seq_t)2)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many qualifying numbers found so far  */
    idx_t m;                  /* candidate integer (fits in idx_t)          */

    for(m = 1; ; ++m)         /* start from 1; sequence offset is 1   */
    {
        if (has_two_in_base3((seq_t)m))   /* check this candidate */
        {
            ++count;
            if (count == n)               /* nth found   */
                return (seq_t)m;          /* return as seq_t            */
        }
    }
}
