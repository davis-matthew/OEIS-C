/*
 * A097924  a(n) = 4*a(n-1) + a(n-2), n>=2, a(0) = 2, a(1) = 7.
 * https://oeis.org/A097924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097924:  a(n) = 4*a(n-1) + a(n-2) with a(0)=2, a(1)=7 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)7;

    /* Compute iteratively; the result fits in seq_t for
       all n for which the value itself fits in seq_t. */
    seq_t prev2 = (seq_t)2;   /* a(n-2) */
    seq_t prev1 = (seq_t)7;   /* a(n-1) */
    seq_t cur;                /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        cur = 4 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
