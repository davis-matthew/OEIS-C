/*
 * A139788  Period 5: repeat [1, 7, 3, 9, 5].
 * https://oeis.org/A139788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (1-4n) mod 10
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t k = (seq_t)n;

    /* Compute 4*k modulo 10 */
    seq_t rem = (4 * k) % 10;

    /* (1 - rem) mod 10, ensuring a positive result */
    seq_t val = (1 + 10 - rem) % 10;

    return val;
}
