/*
 * A069128  Centered 15-gonal numbers: a(n) = (15*n^2 - 15*n + 2)/2.
 * https://oeis.org/A069128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (15*n*n - 15*n + 2)/2
 * Range:     u8 n <= 4, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

extern seq_t a(idx_t n)
{
    /* cast the index to the sequence type first */
    seq_t nn = (seq_t)n;

    /* compute the numerator explicitly to avoid rounding issues */
    seq_t num = (seq_t)15 * nn * nn   /* 15*n^2          */
            - (seq_t)15 * nn         /* -15*n            */
            + (seq_t)2;             /* + 2              */

    /* divide by 2 (even numerator) and return */
    return num / (seq_t)2;
}
