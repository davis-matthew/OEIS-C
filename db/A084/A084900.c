/*
 * A084900  a(n) = 3^(n-2)*n*(5*n+1)/2.
 * https://oeis.org/A084900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^(n-2) * n * (5*n+1) / 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms manually to avoid underflow when n < 2 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Compute 3^(n-2) */
    idx_t exp = n - 2;
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        pow3 *= (seq_t)3;

    /* Compute n * (5*n + 1) / 2 */
    seq_t nn   = (seq_t)n;
    seq_t tmp  = nn * (nn * (seq_t)5 + (seq_t)1);
    tmp /= (seq_t)2;          /* exact division by 2 */

    return pow3 * tmp;        /* final value */
}
