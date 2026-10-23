/*
 * A104584  a(n) = (1/2) * ( 3*n^2 + n*(-1)^n ).
 * https://oeis.org/A104584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(3*n^2 + n*(-1)^n)
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    if ((n & 1ULL) == 0)            /* even n: a(n) = (n/2)*(3*n+1) */
    {
        seq_t half = nseq / 2;
        seq_t term = (seq_t)3 * nseq + 1;
        return half * term;
    }
    else                            /* odd n: a(n) = n*((3*n-1)/2) */
    {
        seq_t term = (seq_t)3 * nseq - 1;
        term = term / 2;
        return nseq * term;
    }
}
