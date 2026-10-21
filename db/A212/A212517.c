/*
 * A212517  Number of (w,x,y,z) with all terms in {1,...,n} and w>2x and
 *          y<=3z.
 * https://oeis.org/A212517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor((n-1)/2) * (n - floor((n-1)/2) - 1)) * * (3 *
 *            floor(n/3) * (floor(n/3)+1)/2 + (n - floor(n/3)) * n)
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 379, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts the pairs (w,x) with w>2x and pairs (y,z) with y<=3z
 * and multiplies them.  The result for n=0,1,2 is 0. */

seq_t a(idx_t n)
{
    if (n <= 2u)
        return 0;

    /* Count of pairs (w,x) with w > 2x */
    idx_t m = (n - 1u) / 2u;
    seq_t f1 = (seq_t)m * (seq_t)(n - m - 1u);

    /* Count of pairs (y,z) with y <= 3z */
    idx_t t = n / 3u;
    seq_t part1 = (seq_t)3 * ((seq_t)t * (t + 1u) / 2u);
    seq_t part2 = (seq_t)(n - t) * n;
    seq_t f2 = part1 + part2;

    return f1 * f2;
}
