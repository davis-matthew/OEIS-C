/*
 * A161731  Expansion of (1-3*x)/(1-8*x+14*x^2).
 * https://oeis.org/A161731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2), a(0) = 1, a(1) = 5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two known initial terms directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)5;

    /* Recurrence: compute iteratively from a(0) and a(1). */
    seq_t prev2 = (seq_t)1;  /* a(n-2) */
    seq_t prev1 = (seq_t)5;  /* a(n-1) */
    seq_t cur   = 0;         /* current a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        cur = (prev1 * (seq_t)8) - (prev2 * (seq_t)14);
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
