/*
 * A028209  Expansion of 1/((1-6x)(1-7x)(1-11x)(1-12x)).
 * https://oeis.org/A028209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36*a(n-1) - 473*a(n-2) + 2682*a(n-3) - 5544*a(n-4) with
 *            initial values a(0)=1, a(1)=36, a(2)=823, a(3)=15282 This
 *            recurrence follows from the generating function
 *            1/((1-6x)(1-7x)(1-11x)(1-12x))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first few terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)36;
    if (n == 2) return (seq_t)823;
    if (n == 3) return (seq_t)15282;

    /* a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)36;
    seq_t a2 = (seq_t)823;
    seq_t a3 = (seq_t)15282;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute the next term using the recurrence. */
        seq_t ai = 36 * a3 - 473 * a2 + 2682 * a1 - 5544 * a0;

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return a3;
}
