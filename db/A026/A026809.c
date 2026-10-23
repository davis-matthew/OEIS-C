/*
 * A026809  a(n) = 3^n*(3^n-1)*(3^n-2)/6.
 * https://oeis.org/A026809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3^n, 3) Recurrence: a(0)=0, a(1)=1, a(2)=84,
 *            a(n)=3*(13*a(n-1)-117*a(n-2)+243*a(n-3)) for n>=3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three values explicitly */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)84;

    /* Iterate using the linear recurrence of order 3 */
    seq_t a0 = (seq_t)0;   /* a(n-3)   */
    seq_t a1 = (seq_t)1;   /* a(n-2)   */
    seq_t a2 = (seq_t)84;  /* a(n-1)   */
    seq_t an = 0;          /* a(n)     */

    for (idx_t i = 3; i <= n; ++i) {
        an = 3 * ((seq_t)13 * a2 - (seq_t)117 * a1 + (seq_t)243 * a0);
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
