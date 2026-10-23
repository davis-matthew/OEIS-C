/*
 * A130861  a(n) = (n-1)*(2*n+5).
 * https://oeis.org/A130861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(2*n+5)
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n = 0 (outside the official offset), although
       the offset is 1 so this case is irrelevant to the sequence. */
    if (n == 0)
        return (seq_t)0;

    /* Promote the index to seq_t before doing arithmetic to avoid
       unintended promotion rules on small integer types. */
    seq_t nn = (seq_t)n;
    return (nn - 1) * (2 * nn + 5);
}
