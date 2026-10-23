/*
 * A173012  a(1) = 1, for n >= 2; a(n) = the smallest number h such that
 *          sigma(h) = A000203(h) = a(n-1) + 2, a(n) = 0 if no such number
 *          exists.
 * https://oeis.org/A173012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less, changed
 * Formula:   a(n) = smallest h such that sigma(h) = a(n-1)+2, or 0 if none.
 *            a(1) = 1
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/sigma_of.h"

/* Return the smallest positive integer h with sigma(h) == target.
   If no such h exists (or if target is too large to search), return 0. */
static seq_t smallest_h(seq_t target)
{
    /* For all target > 2 only search candidates 2 .. target-1 */
    if (target <= (seq_t)2)
        return (seq_t)0;

    /* If target does not fit in the loop counter type, abort. */
    if (target > (seq_t)((idx_t)-1))
        return (seq_t)0;

    idx_t tgt = (idx_t)target;          /* target fits into idx_t */
    for (idx_t cand = 2; cand < tgt; ++cand)
    {
        if (sigma_of(cand) == target)
            return (seq_t)cand;         /* smallest found */
    }
    return (seq_t)0;                    /* no h found */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for n < 1, defensive return */

    seq_t cur = (seq_t)1;               /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t target = cur + (seq_t)2;
        cur = smallest_h(target);
    }
    return cur;
}
