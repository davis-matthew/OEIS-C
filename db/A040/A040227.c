/*
 * A040227  Continued fraction for sqrt(243).
 * https://oeis.org/A040227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 15 if n = 0; otherwise a(n) cycles with period 10: 1, 1,
 *            2, 3, 15, 3, 2, 1, 1, 30 (for n >= 1)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)15;
    static const seq_t period[10] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)15,
        (seq_t)3, (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)30
    };
    /* (n-1) % 10 gives the correct index in the period array */
    return period[(n - 1) % 10];
}
