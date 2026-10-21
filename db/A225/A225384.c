/*
 * A225384  Dimensions of homogeneous components of the universal associative
 *          envelope for a certain nonassociative triple system.
 * https://oeis.org/A225384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=6, a(2)=18; for n≥3, a(n)=6*n*(n-1)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) {
        /* pre‑computed first terms */
        static const seq_t base[3] = {1, 6, 18};
        return base[n];
    }
    /* compute the general formula */
    return (seq_t)6 * (seq_t)n * (seq_t)(n - 1);
}
