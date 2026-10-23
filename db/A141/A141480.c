/*
 * A141480  a(n) = binomial(n+2,3)*5^3.
 * https://oeis.org/A141480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+2,3)*5^3 = (n+2)(n+1)n/6 * 125
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)              /* behaviour below the offset is irrelevant */
        return 0;
    seq_t nseq = (seq_t)n;
    seq_t val = nseq * (nseq + 1) * (nseq + 2) / 6; /* binomial(n+2,3) */
    return val * 125;                                 /* 5^3 = 125 */
}
