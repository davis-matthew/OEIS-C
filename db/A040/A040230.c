/*
 * A040230  Continued fraction for sqrt(246).
 * https://oeis.org/A040230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the nth term of the simple continued fraction expansion
 *            of √246 a(0) = ⌊√246⌋ = 15 For n ≥ 1, the terms repeat with
 *            period 10: 1, 2, 5, 1, 14, 1, 5, 2, 1, 30
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic part after the initial term */
    static const seq_t period[10] = {1, 2, 5, 1, 14, 1, 5, 2, 1, 30};

    if (n == 0)
        return (seq_t)15;

    /* index into the period array */
    idx_t idx = (n - 1) % 10;
    return period[idx];
}
