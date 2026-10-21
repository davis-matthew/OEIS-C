/*
 * A238846  Expansion of (1-2*x)/(1-3*x+x^2)^2.
 * https://oeis.org/A238846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4) for n>3 with
 *            a(0)=1, a(1)=4, a(2)=13, a(3)=40
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)40;

    /* Standard recurrence: iterate up to n */
    seq_t a0 = (seq_t)1;   /* a(i-4)   */
    seq_t a1 = (seq_t)4;   /* a(i-3)   */
    seq_t a2 = (seq_t)13;  /* a(i-2)   */
    seq_t a3 = (seq_t)40;  /* a(i-1)   */
    seq_t an;              /* a(i)     */

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)6 * a3 - (seq_t)11 * a2 + (seq_t)6 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
