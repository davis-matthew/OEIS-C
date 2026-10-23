/*
 * A180356  a(n) = n^6 + 6n.
 * https://oeis.org/A180356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^6 + 6*n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A180356: a(n) = n^6 + 6n, offset 0 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;            /* cast index to seq_t */
    seq_t p = ns * ns;              /* n^2 */
    p *= ns;                         /* n^3 */
    p *= ns;                         /* n^4 */
    p *= ns;                         /* n^5 */
    p *= ns;                         /* n^6 */
    seq_t sixn = ns * (seq_t)6;       /* 6*n */
    return p + sixn;                 /* n^6 + 6n */
}
