/*
 * A097140  Interleave n and 1-n.
 * https://oeis.org/A097140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n/2 if n is even; (3-n)/2 if n is odd
 * Range:     i8 n <= 154, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast n to seq_t only once to avoid repeated casts. */
    seq_t tn = (seq_t)n;

    /* Check the parity of n. */
    if (tn & 1)
    {
        /* n is odd: a = (3 - n) / 2, which is an integer. */
        return ((seq_t)3 - tn) / 2;
    }
    else
    {
        /* n is even: a = n / 2. */
        return tn / 2;
    }
}
