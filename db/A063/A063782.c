/*
 * A063782  a(0) = 1, a(1) = 3; for n > 1, a(n) = 2*a(n-1) + 4*a(n-2).
 * https://oeis.org/A063782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Iteratively compute the sequence up to the requested index.
       The recurrence uses only additions and multiplications by
       constant small integers, so the computation is safe for all
       supported seq_t types. */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = ((seq_t)2 * prev1) + ((seq_t)4 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
