/*
 * A051936  Truncated triangular numbers: a(n) = n*(n+1)/2 - 9.
 * https://oeis.org/A051936
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n*(n+1)/2 - 9
 * Range:     u8 n <= 15, u16 n <= 360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is not defined; return 0. */
    if (n < (idx_t)4)
        return (seq_t)0;

    /* Compute the triangular number first, then subtract 9. */
    seq_t tn = (seq_t)n * ((seq_t)n + (seq_t)1);
    tn /= (seq_t)2;
    tn -= (seq_t)9;
    return tn;
}
