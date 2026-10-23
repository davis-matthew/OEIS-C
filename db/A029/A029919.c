/*
 * A029919  Convert n from inches (") to centimeters (cm).
 * https://oeis.org/A029919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(2.54*n)
 * Range:     u8 n <= 0, u16 n <= 626, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = floor((254*n + 50)/100) for n >= 0 */

seq_t a(idx_t n)
{
    /* Cast the constants to seq_t before multiplication and addition
     * to avoid intermediate overflow when seq_t is small. */
    seq_t mult = (seq_t)254 * n;
    seq_t add  = mult + (seq_t)50;
    return add / (seq_t)100;
}
