/*
 * A040514  Continued fraction for sqrt(538).
 * https://oeis.org/A040514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   continued fraction terms for sqrt(538); a(0)=23, and for n ≥ 1
 *            the terms repeat in the cycle {5, 7, 1, 1, 7, 5, 46}
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;

    static const seq_t period[7] = {5, 7, 1, 1, 7, 5, 46};
    idx_t i = (n - 1) % 7;
    return period[i];
}
