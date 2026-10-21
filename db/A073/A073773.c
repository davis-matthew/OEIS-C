/*
 * A073773  Number of plane binary trees of size n+2 and height n.
 * https://oeis.org/A073773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n < 3, (n^2 - 6) * 2^(n-2) for n >= 3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* Compute (n^2 - 6) in seq_t to avoid overflow in idx_t */
    seq_t sq  = (seq_t)n * (seq_t)n;     /* n^2      */
    seq_t mult = sq - 6;                 /* n^2 - 6  */

    /* Compute 2^(n-2).  For arbitrary‑precision seq_t this is
       just a left shift; for fixed‑width types it works as long
       as the shift count fits within the word size.  */
    seq_t pow2 = ((seq_t)1) << (n - 2);

    return mult * pow2;
}
