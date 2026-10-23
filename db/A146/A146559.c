/*
 * A146559  Expansion of (1-x)/(1 - 2*x + 2*x^2).
 * https://oeis.org/A146559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) for n > 1 a(0) = 1, a(1) = 1
 * Range:     i8 n <= 14, i16 n <= 30, i32 n <= 62, i64 n <= 126,
 *            i128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)1; /* a(n-2), starts with a(0) */
    seq_t a1 = (seq_t)1; /* a(n-1), starts with a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        /* compute an = 2*a1 - 2*a0 using only addition/subtraction */
        seq_t twice_a1 = a1 + a1;
        seq_t twice_a0 = a0 + a0;
        an = twice_a1 - twice_a0;

        /* shift window */
        a0 = a1;
        a1 = an;
    }
    return a1;
}
