/*
 * A179986  Second 9-gonal (or nonagonal) numbers: a(n) = n*(7*n+5)/2.
 * https://oeis.org/A179986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n+5)/2
 * Range:     u8 n <= 8, u16 n <= 136, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                       /* cast index to seq_t */
    seq_t temp = nn * 7 + 5;                  /* compute 7*n + 5 */
    return nn * temp / 2;                     /* n*(7*n+5)/2, integer division exact */
}
