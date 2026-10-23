/*
 * A008881  a(n) = Product_{j=0..5} floor((n+j)/6).
 * https://oeis.org/A008881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{j=0..5} floor((n+j)/6)
 * Range:     u8 n <= 15, u16 n <= 38, u32 n <= 241, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    const seq_t six = (seq_t)6;
    for (idx_t j = 0; j < 6; ++j)
    {
        /* compute floor((n+j)/6) using seq_t arithmetic to avoid
           overflow of the idx_t addition */
        seq_t term = ((seq_t)n + j) / six;
        prod *= term;
    }
    return prod;
}
