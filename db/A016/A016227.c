/*
 * A016227  Expansion of g.f. 1/((1-x)*(1-4*x)*(1-12*x)).
 * https://oeis.org/A016227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1, a(1) = 17
 * Recurrence: a(n) = 16*a(n-1) - 48*a(n-2) + 1   for n >= 2
 */
seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;

    /* Use an iterative loop that keeps only the last two terms. */
    seq_t a0 = (seq_t)1;        /* a(0) */
    seq_t a1 = (seq_t)17;       /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)16 * a1 - (seq_t)48 * a0 + (seq_t)1;
        a0 = a1;
        a1 = an;
    }

    return a1;
}
