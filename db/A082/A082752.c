/*
 * A082752  Duplicate of A023758.
 * https://oeis.org/A082752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 22, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   count_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_leq.h"
/* Sequence A082752: numbers whose binary representation is a non‑empty block
 * of consecutive 1’s followed by zero or more 0’s.  Equivalently, it is the
 * sorted union of the numbers (2^k‑1)·2^m for all integers k≥1 and m≥0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)                       /* undefined for n=0, return 0 */
        return 0;

    /* Find an upper bound H such that f(H) ≥ n.  Start with 1 and double. */
    seq_t high = 1;
    while (count_leq(high, n) < n)
        high <<= 1;

    seq_t low = 1;
    while (low < high)
    {
        seq_t mid = low + ((high - low) >> 1);
        if (count_leq(mid, n) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}
