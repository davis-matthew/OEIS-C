/*
 * A203627  Numbers which are both 9-gonal (nonagonal) and 10-gonal
 *          (decagonal).
 * https://oeis.org/A203627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=1212751, a(n) = 806402*a(n-1) - a(n-2) + 406800 (n
 *            ≥ 3)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 0) {
        /* Undefined for n=0; return 0 as placeholder. */
        return (seq_t)0;
    }
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)1212751;
    }

    seq_t prev2 = (seq_t)1;                 /* a(1) */
    seq_t prev1 = (seq_t)1212751;           /* a(2) */
    seq_t curr;
    for (idx_t i = 3; i <= n; ++i) {
        /* Compute a(i) using the recurrence. */
        curr = (seq_t)806402 * prev1 - prev2 + (seq_t)406800;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
