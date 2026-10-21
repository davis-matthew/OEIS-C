/*
 * A212516  Number of (w,x,y,z) with all terms in {1,...,n} and w>2x and y<3z.
 * https://oeis.org/A212516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor((n-1)/2) * (n - floor((n-1)/2)-1)) * (
 *            ((floor((n+1)/3))*(3*floor((n+1)/3)+1))/2 + (n -
 *            floor((n+1)/3))*n )
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 379, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pairs (w,x) satisfy w > 2x,  pairs (y,z) satisfy y < 3z.  The count equals
   the product of the numbers of such pairs. */

seq_t a(idx_t n)
{
    /* Handle indices below the first non‑zero term. */
    if (n < 3)
        return 0;

    /*  P(n) = sum_{x=1}^{floor((n-1)/2)} (n - 2x)
            = m * (n - (m + 1))  where m = floor((n-1)/2) */
    idx_t m = (n - 1) / 2;
    seq_t P = (seq_t)m * ((seq_t)n - (seq_t)(m + 1));

    /*  Q(n) = sum_{z=1}^{n} min(n, 3z-1)
            = k(3k+1)/2 + (n-k)*n  where k = floor((n+1)/3) */
    idx_t k = (n + 1) / 3;
    seq_t part1 = (seq_t)k * ((seq_t)3 * (seq_t)k + 1) / 2;
    seq_t part2 = (seq_t)(n - k) * (seq_t)n;
    seq_t Q = part1 + part2;

    return P * Q;
}
