/*
 * A239365  Numbers n such that 10*n^2+4 is a square.
 * https://oeis.org/A239365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 38*a(n-1) - a(n-2) Initial values: a(1) = 12, a(2) = 456
 *            This implements the sequence of integers n such that 10*n^2+4 is
 *            a square
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined recursively and can be computed in O(n) time.
       For n >= 1 the values are guaranteed to fit in seq_t.  */
    if (n == 0)
        return (seq_t)0;            /* offset is 1, this value is unused */

    if (n == 1)
        return (seq_t)12;

    if (n == 2)
        return (seq_t)456;

    seq_t prev2 = (seq_t)12;        /* a(1) */
    seq_t prev1 = (seq_t)456;       /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur    = ((seq_t)38 * prev1) - prev2; /* a(i) = 38*a(i-1) - a(i-2) */
        prev2  = prev1;   /* shift window */
        prev1  = cur;
    }
    return prev1;
}
