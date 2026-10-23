/*
 * A010230  Continued fraction for sqrt(188).
 * https://oeis.org/A010230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the (n+1)-th partial quotient of the simple continued
 *            fraction of sqrt(188); a(0) = 13, a(n) for n ≥ 1 repeats the
 *            8‑term period 1,2,2,6,2,2,1,26
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic part for n ≥ 1 */
    static const seq_t period[8] = {
        (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)6,
        (seq_t)2, (seq_t)2, (seq_t)1, (seq_t)26
    };
    if (n == 0)
        return (seq_t)13;           /* a(0) */
    /* (n-1) fits in idx_t; modulo with 8 gives index 0..7 */
    return period[(n - 1) % 8];
}
