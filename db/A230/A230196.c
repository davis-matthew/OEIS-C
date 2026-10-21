/*
 * A230196  Number of pairs (p,q) such that 2*p + 3*q = n and p != q.
 * https://oeis.org/A230196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)/6) + floor((n-1)/6) - floor(n/6) +
 *            floor((n-1)/5) - floor(n/5)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, but guard anyway */

    /* Compute each floor division using unsigned arithmetic */
    seq_t f1 = (seq_t)((n + 1) / 6);  /* floor((n+1)/6)   */
    seq_t f2 = (seq_t)((n - 1) / 6);  /* floor((n-1)/6)   */
    seq_t f3 = (seq_t)(n / 6);        /* floor(n/6)       */
    seq_t f4 = (seq_t)((n - 1) / 5);  /* floor((n-1)/5)   */
    seq_t f5 = (seq_t)(n / 5);        /* floor(n/5)       */

    return f1 + f2 - f3 + f4 - f5;
}
