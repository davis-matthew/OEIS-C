/*
 * A040720  Continued fraction for sqrt(748).
 * https://oeis.org/A040720
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

/* Continued fraction coefficients of sqrt(748).
   The coefficients form a periodic sequence of period 12:
   [27; 2,1,6,5,1,12,1,5,6,1,2,54] and the pattern repeats thereafter. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)27;
    }
    static const seq_t period[12] = {
        (seq_t)2, (seq_t)1, (seq_t)6, (seq_t)5, (seq_t)1, (seq_t)12,
        (seq_t)1, (seq_t)5, (seq_t)6, (seq_t)1, (seq_t)2, (seq_t)54
    };
    idx_t idx = (n - 1) % 12;
    return period[idx];
}
