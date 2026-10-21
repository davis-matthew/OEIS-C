/*
 * A062781  Number of arithmetic progressions of four terms and any mean which
 *          can be extracted from the set of the first n positive integers.
 * https://oeis.org/A062781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * floor(n/3) - (3 * floor(n/3) * (floor(n/3) + 1)) / 2
 * Range:     u8 n <= 26, u16 n <= 452, u32 n <= 750, u64 n <= 750,
 *            u128 n <= 750, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence satisfies a(1)=a(2)=a(3)=0. */
seq_t a(idx_t n)
{
    if (n < 4) return 0;
    seq_t nseq = (seq_t)n;
    seq_t q = nseq / 3;
    seq_t term1 = nseq * q;
    seq_t temp = q * (q + 1);
    temp *= 3;
    temp /= 2;
    return term1 - temp;
}
