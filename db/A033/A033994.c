/*
 * A033994  a(n) = n*(n+1)*(5*n+1)/6.
 * https://oeis.org/A033994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(5*n+1)/6
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = n;                     /* cast index to value type */
    seq_t result = n_t * (n_t + 1);    /* n*(n+1)  */
    result /= 2;                       /* one factor of 2 removed */
    result *= (5 * n_t + 1);           /* multiply by (5*n+1) */
    result /= 3;                       /* remaining factor of 3 removed */
    return result;                     /* final value a(n) */
}
