/*
 * A212515  Number of (w,x,y,z) with all terms in {1,...,n} and w<=2x and
 *          y>=3z.
 * https://oeis.org/A212515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (sum_{x=1}^n min(n,2x)) * (sum_{z=1}^{⌊n/3⌋} (n - 3z +
 *            1)) = (⌊n/2⌋(⌊n/2⌋+1) + n(n-⌊n/2⌋)) * (⌊n/3⌋(n+1) -
 *            3*⌊n/3⌋(⌊n/3⌋+1)/2) with a(0)=a(1)=a(2)=0
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 430, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_t = (seq_t)n;
    seq_t m = (seq_t)(n / 2);          /* floor(n/2)   */
    seq_t Sx = m * (m + 1) + n_t * (n_t - m);
    seq_t k = (seq_t)(n / 3);          /* floor(n/3)   */
    seq_t Sy = k * (n_t + 1) - ((seq_t)3 * k * (k + 1)) / 2;
    return Sx * Sy;
}
