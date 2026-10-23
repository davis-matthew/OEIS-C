/*
 * A199834  Number of -n..n arrays x(0..4) of 5 elements with zero sum and no
 *          two neighbors summing to zero.
 * https://oeis.org/A199834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (115/12)*n^4 - (29/6)*n^3 + (5/12)*n^2 - (7/6)*n
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 102, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid overflow during multiplication */
    seq_t sq  = (seq_t)n;
    seq_t n2  = sq * sq;          /* n^2 */
    seq_t n3  = n2 * sq;          /* n^3 */
    seq_t n4  = n2 * n2;          /* n^4 */

    seq_t t1 = ((seq_t)115 * n4) / 12;  /* (115/12)*n^4 */
    seq_t t2 = ((seq_t)29 * n3) / 6;    /* (29/6)*n^3 */
    seq_t t3 = ((seq_t)5 * n2) / 12;    /* (5/12)*n^2 */
    seq_t t4 = ((seq_t)7 * sq) / 6;     /* (7/6)*n */

    /* The expression is guaranteed to be non‑negative, but we keep
       the subtraction order so the intermediate result does not
       underflow for an unsigned seq_t. */
    return t1 - t2 + t3 - t4;
}
