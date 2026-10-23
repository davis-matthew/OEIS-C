/*
 * A040347  Continued fraction for sqrt(367).
 * https://oeis.org/A040347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction for sqrt(367). a(0) = floor(sqrt(367)), a(n)
 *            for n>0 taken from the repeating period of length 20
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)19;      /* a(0) */
    static const seq_t period[20] = {
        (seq_t)6, (seq_t)2, (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)1,
        (seq_t)2, (seq_t)1, (seq_t)12, (seq_t)19, (seq_t)12, (seq_t)1,
        (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)2,
        (seq_t)6, (seq_t)38
    };
    /* Index into the period: (n-1) mod 20 */
    return period[(n - 1) % 20];
}
