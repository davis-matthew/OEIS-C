/*
 * A222409  Numbers of the form 8n + [0,3,6,4,7].
 * https://oeis.org/A222409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * (n / 5) + [0, 3, 6, 4, 7][n % 5]
 * Range:     u8 n <= 159, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the integer part and remainder when n is divided by 5 */
    idx_t k = n / 5;
    unsigned r = n % 5;          /* 0 <= r < 5, fits in an unsigned */

    /* Constants for the offsets */
    static const seq_t OFFSETS[5] = { 0, 3, 6, 4, 7 };

    /* Calculate the value using seq_t arithmetic to avoid overflow */
    return (seq_t)k * (seq_t)8 + OFFSETS[r];
}
