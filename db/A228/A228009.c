/*
 * A228009  The number of n-digit numbers whose first k digits are divisible
 *          by k^2 for k = 1..n.
 * https://oeis.org/A228009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the number of n‑digit numbers whose first k digits are divisible by k^2
   for every k = 1..n.  The algorithm builds the numbers digit by digit and
   prunes prefixes that violate the divisibility condition, so the work is
   proportional to the number of admissible prefixes (≤ 86 in total).  For n > 7
   the result is 0 because no such numbers exist.  */
#include <stdbool.h>

static seq_t dfs(idx_t pos, seq_t prefix, idx_t n)
{
    if (pos > n)          /* a full n‑digit number has been built */
        return 1;
    seq_t count = 0;
    seq_t start = (pos == 1) ? 1 : 0;   /* leading digit cannot be 0 */
    seq_t pos_sq = (seq_t)pos * (seq_t)pos;
    for (seq_t d = start; d <= 9; ++d) {
        seq_t newp = prefix * 10 + d;
        if (newp % pos_sq == 0)
            count += dfs(pos + 1, newp, n);
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 only */
    if (n > 7) return 0;           /* no admissible numbers beyond 7‑digits */
    return dfs(1, 0, n);
}
