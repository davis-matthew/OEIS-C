/*
 * A138002  a(3*n)=2*n, a(3*n+1)=a(n)+n, a(3*n+2)=2*a(n), a(n)=0 for n<3.
 * https://oeis.org/A138002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(3*n) = 2*n, a(3*n+1) = a(n) + n, a(3*n+2) = 2*a(n)
 *            (equivalently a(n) = (n mod 3)*a(n/3) + (2 - n mod 3)*(n/3) for
 *            n >= 3, with a(0) = a(1) = a(2) = 0)
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases for indices below the offset */
    if (n < 3)
        return (seq_t)0;

    /* Decompose n into quotient and remainder when divided by 3 */
    idx_t  q = n / 3;          /* floor(n/3) */
    unsigned r = n % 3;        /* n mod 3 */

    /* Recursive call on the smaller index q */
    seq_t aq = a(q);

    /* Combine according to the recurrence */
    if (r == 0)
        return (seq_t)2 * (seq_t)q;          /* a(3*q) = 2*q          */
    else if (r == 1)
        return aq + (seq_t)q;                /* a(3*q+1) = a(q)+q     */
    else /* r == 2 */
        return aq * (seq_t)2;                /* a(3*q+2) = 2*a(q)     */
}
