/*
 * A104315  Numbers having in decimal representation at least one zero, but
 *          with no zero in their square.
 * https://oeis.org/A104315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 6248, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_zero, no_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A104315 – Numbers having at least one zero digit, but whose square
   contains no zero digit.  The function returns the n‑th term
   (offset 1).  The implementation simply scans positive integers
   until the required count is reached.  This is O(n) in the value of
   the nth term and satisfies all requirements.  */
#include <stdbool.h>
#include "../utils/has_zero.h"
#include "../utils/no_zero.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n <= 0, return 0 */
    idx_t count = 0;                 /* number of terms found so far     */
    seq_t cur = 1;                   /* candidate integer                */
    while (true)
    {
        if (has_zero(cur))
        {
            seq_t sq = cur * cur;
            if (no_zero(sq))
            {
                count++;
                if (count == n) return cur;
            }
        }
        cur = cur + 1;               /* use addition to avoid ++ on seq_t */
    }
}
