/*
 * A034789  Related to sextic factorial numbers A008542.
 * https://oeis.org/A034789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6^(n-1)*A008542(n)/n! (recurrence: a(1)=1, n*a(n) +
 *            6*(-6*n+5)*a(n-1) = 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; 0 is undefined */
    seq_t result = 1;              /* a(1) = 1 */
    for (idx_t k = 2; k <= n; ++k)
    {
        /* factor = 6*(6*k-5) */
        seq_t k_seq = (seq_t)k;
        seq_t factor = (seq_t)6 * ((seq_t)6 * k_seq - (seq_t)5);
        /* a(k) = a(k-1) * factor / k */
        result = result * factor / k_seq;
    }
    return result;
}
