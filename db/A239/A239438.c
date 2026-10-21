/*
 * A239438  Maximal number of points that can be placed on a triangular grid
 *          of side n so that there is no pair of adjacent points.
 * https://oeis.org/A239438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*(n+1)/6) for n > 5; a(1)=1, a(2)=1, a(3)=3,
 *            a(4)=4, a(5)=6
 * Range:     u8 n <= 38, u16 n <= 626, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 5)
    {
        static const seq_t first[5] = { (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)4, (seq_t)6 };
        return first[n - 1];
    }
    /* Compute ceil(n*(n+1)/6) using 64‑bit arithmetic to avoid overflow. */
    idx_t prod = n * (n + 1);
    idx_t val  = (prod + 5) / 6; /* (highest remainder + 5) ensures ceiling */
    return (seq_t)val;
}
