/*
 * A212518  Number of (w,x,y,z) with all terms in {1,...,n} and w>2x and y>3z.
 * https://oeis.org/A212518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∑_{x=1}^{⌊n/2⌋} (n-2x)) * (∑_{z=1}^{⌊n/3⌋} (n-3z))
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 567, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* number of pairs (w,x) with 1 ≤ w,x ≤ n and w > 2x */
    idx_t m1 = n / 2;                     /* floor(n/2) handles all n */
    seq_t N  = (seq_t)n;
    seq_t cnt1 = (seq_t)m1 * (N - (seq_t)m1 - 1);

    /* number of pairs (y,z) with 1 ≤ y,z ≤ n and y > 3z */
    idx_t m2 = n / 3;                     /* floor(n/3) handles all n */
    seq_t cnt2 = (seq_t)m2 * N
                 - ((seq_t)3 * (seq_t)m2 * ((seq_t)m2 + 1)) / 2;

    return cnt1 * cnt2;
}
