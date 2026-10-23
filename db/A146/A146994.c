/*
 * A146994  a(n) = (n+1)^2/4 + (floor((n+5)/6) - 1/4) * ((n+1) mod 2).
 * https://oeis.org/A146994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(2*n-1) = n^2 * a(2*n) = n*(n+1) + floor((2*n+5)/6)
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based (offset 1).  If n is 0, return 0 (undefined for this offset). */
    if (n == 0) return 0;

    /* Check parity of n */
    if ((n & 1ULL) == 1ULL)     /* n is odd: n = 2*k-1 => k = (n+1)/2 */
    {
        seq_t k = ((seq_t)(n + 1)) >> 1;   /* (n+1)/2 */
        return k * k;                       /* k^2 */
    }
    else                         /* n is even: n = 2*k */
    {
        seq_t k = (seq_t)(n >> 1);          /* k = n/2 */
        seq_t term = k * (k + 1);           /* k*(k+1) */
        term += ( (2 * k + 5) / 6 );        /* + floor((2*k+5)/6) */
        return term;
    }
}
