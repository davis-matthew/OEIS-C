/*
 * A091626  Number of ordered integer pairs (b,c) with 0 <= b, c <= n such
 *          that both roots of x^2+bx+c=0 are integers.
 * https://oeis.org/A091626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A091626
 *  Number of ordered integer pairs (b,c) with 0 ≤ b,c ≤ n such that
 *  both roots of x^2 + b x + c = 0 are integers.
 *  Equivalent counting of unordered nonnegative pairs (i,j) with
 *  i + j ≤ n and i·j ≤ n. */
seq_t a(idx_t n)
{
    seq_t ans = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        idx_t limit;
        if (i == 0)
        {
            /* product constraint is always satisfied when i = 0 */
            limit = n;            /* j ≤ n-i = n */
        }
        else
        {
            idx_t l1 = n - i;     /* from i + j ≤ n  */
            idx_t l2 = n / i;     /* from i · j ≤ n  */
            limit = (l1 < l2) ? l1 : l2;
        }

        if (limit < i)
            continue;                 /* no j ≥ i satisfies constraints */

        /* Count j from i to limit inclusive */
        ans += (seq_t)(limit - i + 1);
    }
    return ans;
}
