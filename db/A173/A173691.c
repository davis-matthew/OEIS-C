/*
 * A173691  Partial sums of round(n^2/6).
 * https://oeis.org/A173691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((2*n^3 + 3*n^2 + 6*n)/36) (half‑up rounding)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1293, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* conversion to seq_t */
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t n3 = n2 * nn;            /* n^3 */
    
    /* Compute X = 2*n^3 + 3*n^2 + 6*n */
    seq_t X = (seq_t)2 * n3 + (seq_t)3 * n2 + (seq_t)6 * nn;
    
    /* Round half‑up: (X + 18) / 36  (because 18 = 36/2) */
    seq_t res = (X + (seq_t)18) / (seq_t)36;
    
    return res;
}
