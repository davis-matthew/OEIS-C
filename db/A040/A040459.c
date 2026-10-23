/*
 * A040459  Continued fraction for sqrt(481).
 * https://oeis.org/A040459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 21; for n >= 1: a(n) = period[(n-1) mod 13]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period: [1,13,1,1,1,4,4,1,1,1,13,1,42] */
seq_t a(idx_t n)
{
    static const seq_t period[13] = {
        (seq_t)1, (seq_t)13, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)4, (seq_t)4, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)13, (seq_t)1, (seq_t)42
    };
    if (n == 0)
        return (seq_t)21;
    return period[(n - 1) % 13];
}
