/*
 * A152108  a(n) = ((7+sqrt(5))^n + (7-sqrt(5))^n)/2.
 * https://oeis.org/A152108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 44*a(n-2), a(0)=1, a(1)=7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)7;

    /* Handle first two indices */
    if (n == 0) return a0;
    if (n == 1) return a1;

    seq_t an;
    /* Iterate up to n using the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)14 * a1 - (seq_t)44 * a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}
