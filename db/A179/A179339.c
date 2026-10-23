/*
 * A179339  Positive integers of the form (30*m^2+1)/11.
 * https://oeis.org/A179339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (330*n*(n-1)+45*(2*n-1)*(-1)^n+89)/4
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 3613, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;

    /* Compute 330*n*(n-1) + 89          */
    seq_t term = (seq_t)330 * nseq * (nseq - 1) + 89;

    /* Compute 45*(2*n-1)                */
    seq_t temp = (seq_t)45 * ((seq_t)2 * nseq - 1);

    /* Add or subtract according to (-1)^n  */
    if ((n & 1) == 0)          /* n even => (+1) */
        term += temp;
    else                       /* n odd  => (-1) */
        term -= temp;

    /* Final division by 4             */
    return term / 4;
}
