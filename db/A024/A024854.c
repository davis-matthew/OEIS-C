/*
 * A024854  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n-k+1), where k =
 *          floor(n/2), s = (natural numbers), t = (natural numbers >= 3).
 * https://oeis.org/A024854
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} i*(n-i+3) = (n+3)*k*(k+1)/2 -
 *            k*(k+1)*(2*k+1)/6, where k = floor(n/2)
 * Range:     u8 n <= 13, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* values below the OEIS offset are undefined */

    seq_t k = n / 2;                /* floor(n/2) */
    seq_t c1 = (seq_t)n + 3;        /* n + 3 */

    /* term1 = (n+3)*k*(k+1)/2 */
    seq_t a1 = k;
    seq_t b1 = k + 1;
    if ((a1 & (seq_t)1) == 0)       /* divide the even factor by 2 */
        a1 /= 2;
    else
        b1 /= 2;
    seq_t term1 = c1 * a1;
    term1 = term1 * b1;

    /* term2 = k*(k+1)*(2*k+1)/6 */
    seq_t a2 = k;
    seq_t b2 = k + 1;
    seq_t c2 = (seq_t)2 * k + 1;
    if ((a2 & (seq_t)1) == 0)
        a2 /= 2;
    else
        b2 /= 2;
    if (a2 % (seq_t)3 == 0)
        a2 /= 3;
    else if (b2 % (seq_t)3 == 0)
        b2 /= 3;
    else
        c2 /= 3;
    seq_t term2 = a2 * b2;
    term2 = term2 * c2;

    return term1 - term2;
}
