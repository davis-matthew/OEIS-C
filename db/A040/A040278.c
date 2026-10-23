/*
 * A040278  Continued fraction for sqrt(296).
 * https://oeis.org/A040278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A040278: continued fraction coefficients for sqrt(296).
   a(0) = 17; for n >= 1 the coefficients repeat the pattern
   [4, 1, 7, 1, 4, 34].  This implements the sequence exactly. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)17;

    /* Periodic part of the continued fraction. */
    static const seq_t period[6] = {4, 1, 7, 1, 4, 34};

    /* (n-1) % 6 gives the position in the period (0‑based). */
    idx_t p = (n - 1) % 6;
    return period[p];
}
