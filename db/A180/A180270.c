/*
 * A180270  Integers of the form (k^12 - k^8 - k^4 + 1)/512.
 * https://oeis.org/A180270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n-1)^4 + 1) * (( (2*n-1)-1) * ((2*n-1)+1) *
 *            ((2*n-1)^2 + 1) )^2 / 512
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 34, u128 n <= 1367,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t k = (seq_t)n * 2;
    k -= 1;                     /* k = 2*n-1 */
    seq_t k2 = k * k;           /* k^2 */
    seq_t k4 = k2 * k2;          /* k^4 */
    seq_t d = (k - 1) * (k + 1); /* (k-1)*(k+1) */
    d = d * (k2 + 1);            /* * (k^2+1) */
    d = d / 16;                  /* divide by 16 */
    seq_t o = (k4 + 1) / 2;      /* (k^4+1)/2 */
    seq_t res = d * d;
    res = res * o;
    return res;
}
