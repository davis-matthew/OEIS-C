/*
 * A049031  Maximization of sums of cubes of integer differences (b_[ i ]-i)^3
 *          over permutations {b_[ i ], for i-1,2,...,n} on first n integers.
 * https://oeis.org/A049031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = r*(n-r+1)*(n-r)*(n-r-1)/3, where r = floor((n+1)/4)
 * Range:     u8 n <= 7, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined below offset, but keeping safe */

    idx_t r = (n + 1) / 4;          /* integer division is floor */
    if (r == 0)
        return 0;                   /* handles small n where r = 0 */

    seq_t sr = (seq_t)r;
    seq_t s1 = (seq_t)(n - r + 1);
    seq_t s2 = (seq_t)(n - r);
    seq_t s3 = (seq_t)(n - r - 1);

    seq_t prod = sr * s1 * s2 * s3; /* multiplication in seq_t */
    return prod / 3;               /* exact division by 3 */
}
