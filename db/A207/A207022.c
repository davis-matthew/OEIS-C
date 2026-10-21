/*
 * A207022  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 107, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207022: Number of nX6 0..1 arrays avoiding
   horizontally 0 0 0 and 0 1 1, and vertically 0 0 1 and 1 0 1.
   Empirical formula:
   a(n) = (33*n^4 + 90*n^3 + 75*n^2 - 126*n) / 4
*/
seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;          /* cast index to seq_t           */
    seq_t n2  = nn * nn;           /* n^2                            */
    seq_t n3  = n2 * nn;           /* n^3                            */
    seq_t n4  = n2 * n2;           /* n^4                            */
    seq_t val = 33 * n4 + 90 * n3 + 75 * n2 - 126 * nn; /* numerator */
    return val / 4;                /* integer division by 4          */
}
