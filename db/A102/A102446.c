/*
 * A102446  a(n) = a(n-1) + 4*a(n-2) for n>1, a(0) = a(1) = 2.
 * https://oeis.org/A102446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2); a(0) = 2, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the two base cases directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)2;

    seq_t prevprev = (seq_t)2;  /* a(0) */
    seq_t prev     = (seq_t)2;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = prev + (seq_t)4 * prevprev;  /* a(i) = a(i-1) + 4*a(i-2) */
        prevprev = prev;
        prev = curr;
    }

    return prev;  /* prev holds a(n) after the loop */
}
