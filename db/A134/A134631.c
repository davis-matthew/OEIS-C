/*
 * A134631  a(n) = 5*n^5 - 3*n^3 + 2*n^2. Coefficients and exponents are the
 *          prime numbers in decreasing order.
 * https://oeis.org/A134631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^5 - 3*n^3 + 2*n^2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate type to avoid overflow while
       computing the polynomial. The cast to seq_t may truncate
       beyond the size of seq_t, which is acceptable because only
       terms that fit in seq_t are checked. */
    __int128 x   = n;          /* n as 128‑bit integer */
    __int128 n2  = x * x;      /* n^2 */
    __int128 n3  = n2 * x;     /* n^3 */
    __int128 n5  = n3 * n2;    /* n^5 = n^3 * n^2 */
    __int128 val = 5 * n5 - 3 * n3 + 2 * n2;
    return (seq_t)val;
}
