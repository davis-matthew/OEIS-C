/*
 * A048500  a(n) = 2^(n-1)*(7*n-12) + 7.
 * https://oeis.org/A048500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 4*a(n-3); initial values: a(0)=1,
 *            a(1)=2, a(2)=11
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)11;

    seq_t a0 = 1;    /* a(n-3)   */
    seq_t a1 = 2;    /* a(n-2)   */
    seq_t a2 = 11;   /* a(n-1)   */
    seq_t a;

    for (idx_t i = 3; i <= n; ++i) {
        a = 5*a2 - 8*a1 + 4*a0;
        a0 = a1;
        a1 = a2;
        a2 = a;
    }
    return a2;
}
