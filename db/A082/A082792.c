/*
 * A082792  Smallest multiple of n beginning with 3.
 * https://oeis.org/A082792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest positive multiple of n whose decimal
 *            representation starts with the digit 3. This is found by
 *            scanning over the number of digits d: for each d, let low =
 *            3·10^d and high = 4·10^d – 1. The first k such that k·n ∈
 *            [low,high] gives the required value
 * Range:     u8 n <= 13, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;       /* convert index to the same integer type */
    seq_t pow10 = 1;             /* 10^d, starting with d = 0 */

    while (1) {
        /* low = 3*10^d, high = 4*10^d - 1 */
        seq_t low  = (seq_t)3 * pow10;
        seq_t high = ((seq_t)4 * pow10) - 1;

        /* smallest k with k*n ≥ low :  k = ceil(low/n) = (low + n - 1)/n */
        seq_t k = (low + nseq - 1) / nseq;
        seq_t m = k * nseq;

        if (m <= high)
            return m;          /* m starts with 3 and is minimal */

        pow10 *= 10;          /* move to the next higher digit length */
    }
}
