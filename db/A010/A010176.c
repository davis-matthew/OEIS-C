/*
 * A010176  Continued fraction for sqrt(111).
 * https://oeis.org/A010176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction expansion of sqrt(111)
   a(0) = 10
   for n > 0: a(n) = {1, 1, 6, 1, 1, 20}[(n-1) % 6] */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)10;

    static const seq_t period[6] = {
        (seq_t)1, (seq_t)1, (seq_t)6,
        (seq_t)1, (seq_t)1, (seq_t)20
    };

    idx_t idx = (n - (idx_t)1) % (idx_t)6;
    return period[idx];
}
