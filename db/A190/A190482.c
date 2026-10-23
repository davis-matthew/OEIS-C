/*
 * A190482  Convex, obtuse, hexagonal lattice numbers.
 * https://oeis.org/A190482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 11 for n >= 7; the first six terms are 7, 10, 12, 13,
 *            14, 16
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is simply consecutive integers starting at 18 (n+11) after n=6. */
    static const seq_t first6[6] = { (seq_t)7, (seq_t)10, (seq_t)12,
                                    (seq_t)13, (seq_t)14, (seq_t)16 };

    if (n <= 6)
        return first6[n - 1];
    else
        return (seq_t)n + (seq_t)11;
}
