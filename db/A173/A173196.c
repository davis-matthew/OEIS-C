/*
 * A173196  Partial sums of A002620.
 * https://oeis.org/A173196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 - 2*n)/24)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1701, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the sequence type once to avoid repeated casts */
    seq_t nn = (seq_t)n;

    /* compute n^2 and n^3 using seq_t arithmetic only */
    seq_t n_sq = nn * nn;
    seq_t n_cu = n_sq * nn;

    /* numerically: 2*n^3 + 3*n^2 - 2*n */
    seq_t numer = 2 * n_cu + 3 * n_sq - 2 * nn;

    /* the denominator is 24; division truncates toward 0 which is
       equivalent to floor for non‑negative numerators */
    return numer / 24;
}
