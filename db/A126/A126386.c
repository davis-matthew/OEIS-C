/*
 * A126386  Number of base 32 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A126386: number of base‑32 n‑digit numbers whose adjacent digits differ by
   at most 1.  The values are obtained by dynamic programming over the
   32‑state graph; a(n) = sum_{d=0}^{31} f(n,d), where
   f(1,d)=1 and f(k+1,d)=f(k,d)+f(k,d-1)+f(k,d+1) (with missing
   neighbours treated as 0). */

seq_t a(idx_t n)
{
    /* a(0) = 1 (empty string) */
    if (n == 0u)
        return (seq_t)1;

    /* Tables for DP (fixed size 32, no heap allocation needed). */
    seq_t cur[32];
    seq_t next[32];

    /* Base case: length 1, one sequence for each starting digit. */
    for (idx_t d = 0u; d < 32u; ++d)
        cur[d] = (seq_t)1;

    /* Sum for length 1. */
    seq_t sum = (seq_t)0;
    for (idx_t d = 0u; d < 32u; ++d)
        sum += cur[d];

    if (n == 1u)
        return sum;

    /* DP for lengths 2 .. n. */
    for (idx_t step = 2u; step <= n; ++step)
    {
        for (idx_t d = 0u; d < 32u; ++d)
        {
            seq_t val = cur[d];
            if (d > 0u)
                val += cur[d - 1u];
            if (d < 31u)
                val += cur[d + 1u];
            next[d] = val;
        }

        /* Compute the sum for the current length. */
        sum = (seq_t)0;
        for (idx_t d = 0u; d < 32u; ++d)
            sum += next[d];

        /* Prepare for the next iteration: copy next → cur. */
        for (idx_t d = 0u; d < 32u; ++d)
            cur[d] = next[d];
    }

    return sum;
}
