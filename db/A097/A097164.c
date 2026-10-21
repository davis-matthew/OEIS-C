/*
 * A097164  Expansion of (1+3x)/((1-x)(1-4x^2)).
 * https://oeis.org/A097164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3), with a(0)=1, a(1)=4, a(2)=8
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;

    /* Keep the last three computed values. */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)4;   /* a(n-2) */
    seq_t a2 = (seq_t)8;   /* a(n-1) */
    seq_t a_next;

    /* Iterate until the desired index.  The loop runs at most n-2 times. */
    for (idx_t i = 3; i <= n; ++i)
    {
        a_next = a2 + (seq_t)4 * a1 - (seq_t)4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }
    return a2;
}
