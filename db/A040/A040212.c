/*
 * A040212  Continued fraction for sqrt(228).
 * https://oeis.org/A040212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(228)
   a(0) = 15; for n ≥ 1: a(n) = 10 if n is odd, otherwise a(n) = 30
   (periodic sequence with period length 2 after the first term) */

seq_t a(idx_t n)
{
    /* first term */
    if (n == 0) {
        return (seq_t)15;
    }

    /* periodic part: odd indices → 10, even indices → 30 */
    return (n & (idx_t)1) ? (seq_t)10 : (seq_t)30;
}
