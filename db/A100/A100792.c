/*
 * A100792  Group the natural numbers so that the n-th group contains n(n+1)/2
 *          = T(n) terms: (1), (2,3,4), (5,6,7,8,9,10),
 *          (11,12,13,14,15,16,17,18,19,20),(21,22,23,24,25,26,27,28,29,30,31,32,33,34,35),...
 *          The r-th term of the n-th row of the following triangle is the sum
 *          of the next r terms of the n-th grou...
 * https://oeis.org/A100792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = sum of the next r terms of the g‑th group, where g is the
 *            smallest integer with g(g+1)/2 ≥ n, r = n – (g−1)g/2,
 *            group_start = 1 + (g–1)g(g+1)/6, offset = (r–1)r/2, a(n) =
 *            sum_{k=0}^{r−1} (group_start + offset + k) =
 *            ((2·(group_start+offset)+r−1) * r)/2
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find group g */
    idx_t g = 1;
    while ((g * (g + 1ULL) / 2ULL) < n) ++g;

    idx_t prevS = (g - 1) * g / 2;          /* terms before group g in the sequence */
    idx_t r     = n - prevS;                /* row index within group g    */

    /* compute starting number of group g */
    seq_t gs = 1 + (seq_t)(g - 1) * g * (g + 1) / 6;

    /* offset into the group where the r‑th block starts */
    seq_t off = (seq_t)(r - 1) * r / 2;

    /* first term of the summation block */
    seq_t A = gs + off;

    /* sum of r consecutive integers starting at A   */
    seq_t temp = ( (seq_t)2 * A + (seq_t)(r - 1) );
    seq_t result = temp * r / 2;            /* exact division */

    return result;
}
