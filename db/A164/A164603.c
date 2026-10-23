/*
 * A164603  a(n) = ((1+4*sqrt(2))*(2+2*sqrt(2))^n +
 *          (1-4*sqrt(2))*(2-2*sqrt(2))^n)/2.
 * https://oeis.org/A164603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) for n > 1; a(0) = 1, a(1) = 18
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)18;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)18;  /* a(1) */
    seq_t an = a0;         /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)4 * a1 + (seq_t)4 * a0;  /* recurrence */
        a0 = a1;
        a1 = an;
    }
    return an;
}
