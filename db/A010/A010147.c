/*
 * A010147  Continued fraction for sqrt(67).
 * https://oeis.org/A010147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(0) = 8 For n > 0, a(n) repeats the period [5, 2, 1, 1, 7, 1,
 *            1, 2, 5, 16]
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 94 terms through n = 9798 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first term */
    if (n == 0)
        return (seq_t)8;

    /* period of length 10 for the continued fraction of sqrt(67) */
    static const seq_t period[10] = {
        (seq_t)5, (seq_t)2, (seq_t)1, (seq_t)1,
        (seq_t)7, (seq_t)1, (seq_t)1, (seq_t)2,
        (seq_t)5, (seq_t)16
    };

    /* index into the period: (n-1) modulo 10 */
    idx_t idx = (n - 1) % (idx_t)10;
    return period[idx];
}
