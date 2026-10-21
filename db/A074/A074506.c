/*
 * A074506  a(n) = 1^n + 3^n + 4^n.
 * https://oeis.org/A074506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 3^n + 4^n Recurrence: a(0)=3, a(1)=8,
 *            a(n)=7*a(n-1)-12*a(n-2)+6 (n≥2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)8;

    /* Iteratively compute using the linear recurrence */
    seq_t a0 = (seq_t)3;   /* a(n-2) */
    seq_t a1 = (seq_t)8;   /* a(n-1) */
    seq_t an;              /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)7 * a1 - (seq_t)12 * a0 + (seq_t)6;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
