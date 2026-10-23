/*
 * A040392  Continued fraction for sqrt(413).
 * https://oeis.org/A040392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 20; for n > 0, a(n) = [3, 9, 1, 4, 1, 9, 3, 40][(n-1) mod
 *            8]
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continues the periodic continued‑fraction expansion of sqrt(413). */

seq_t a(idx_t n)
{
    /* The first term of sqrt(413) is 20.  All subsequent terms repeat
       with period 8: 3, 9, 1, 4, 1, 9, 3, 40. */
    if (n == 0)
        return (seq_t)20;

    /* Periodic part.  The type seq_t may be any integral or arbitrary‑precision
       type, so cast the integer constants to seq_t. */
    static const seq_t period[8] = {
        (seq_t)3, (seq_t)9, (seq_t)1, (seq_t)4,
        (seq_t)1, (seq_t)9, (seq_t)3, (seq_t)40
    };

    /* n-1 is safe because we already handled the case n==0. */
    return period[(n - 1) % 8];
}
