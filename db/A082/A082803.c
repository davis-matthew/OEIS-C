/*
 * A082803  Smallest multiple of 8 beginning with n.
 * https://oeis.org/A082803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest integer ≥ n·10^k and < (n+1)·10^k
 *          that is divisible by 8.  Search for the smallest k ≥ 0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to sequence type */

    for (seq_t pow10 = 1; ; pow10 *= 10)   /* iterate 1, 10, 100, 1000, … */
    {
        seq_t L = nn * pow10;                   /* lower bound of range */
        seq_t U = (nn + 1) * pow10 - 1;         /* upper bound of range */

        /* smallest multiple of 8 that is ≥ L */
        seq_t div  = (L + 7) / 8;                /* ceil(L / 8) */
        seq_t cand = div * 8;

        if (cand <= U)
            return cand;                        /* first range that works */
    }
}
