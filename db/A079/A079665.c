/*
 * A079665  Triangular array read by rows: row s contains integers of the form
 *          (2^s+1)/(2^r+1) in order of increasing r <= s-1.
 * https://oeis.org/A079665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (2^s + 1)/(2^r + 1) where s ≥ 2, 1 ≤ r < s, s % r == 0
 *            and (s / r) is odd. The terms are listed by increasing s, and
 *            for each fixed s by increasing r
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  The offset of the sequence is 1. */
    if (n == 0) return 0;   /* undefined, guard against misuse */

    idx_t count = 0;        /* number of terms produced so far      */

    for (idx_t s = 1; ; ++s)          /* iterate over rows (s) */
    {
        seq_t num = pow2(s) + 1;       /* N = 2^s + 1                      */

        for (idx_t r = 1; r < s; ++r)  /* iterate over columns (r)      */
        {
            /* (s/r) odd  &&  r divides s  -> integer term exists   */
            if ((s % r == 0) && ((s / r) & 1U))
            {
                ++count;
                if (count == n)
                {
                    seq_t denom = pow2(r) + 1;
                    return num / denom; /* exact division */
                }
            }
        }
    }
}
