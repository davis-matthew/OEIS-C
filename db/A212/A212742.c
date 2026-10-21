/*
 * A212742  Number of (w,x,y,z) with all terms in {0,...,n} and
 *          max{w,x,y,z}<=2*min{w,x,y,z}.
 * https://oeis.org/A212742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor(n/2)+1)^4 + (ceil(n/2))^4
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t mid = n / 2;
    seq_t m = (seq_t)mid;           /* cast to seq_t for arithmetic */

    /* (m+1)^4 */
    seq_t term1 = m + 1;
    term1 = term1 * term1;          /* (m+1)^2   */
    term1 = term1 * term1;          /* (m+1)^4   */

    /* ceil(n/2) = n - floor(n/2) */
    seq_t q = (seq_t)(n - mid);
    seq_t term2 = q;
    term2 = term2 * term2;          /* q^2 */
    term2 = term2 * term2;          /* q^4 */

    return term1 + term2;
}
