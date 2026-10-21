/*
 * A098648  Expansion of (1-3*x)/(1 - 6*x + 4*x^2).
 * https://oeis.org/A098648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(3*a(n-1) - 2*a(n-2)), a(0) = 1, a(1) = 3; (equivalent
 *            to a(n) = 6*a(n-1) - 4*a(n-2))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Iterative computation using the recurrence a(n) = 6*a(n-1) - 4*a(n-2) */
    seq_t a0 = (seq_t)1;           /* a(n-2) */
    seq_t a1 = (seq_t)3;           /* a(n-1) */
    seq_t an;                      /* a(n) */
    const seq_t six = (seq_t)6;
    const seq_t four = (seq_t)4;

    for (idx_t i = 2; i <= n; ++i) {
        an = six * a1 - four * a0;
        a0 = a1;
        a1 = an;
    }

    return a1;
}
