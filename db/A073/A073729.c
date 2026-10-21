/*
 * A073729  Concatenation of initial and final digits of n in decimal
 *          representation.
 * https://oeis.org/A073729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = A000030(n)*10 + A010879(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenation of the first and last decimal digits of n */

seq_t a(idx_t n)
{
    /* find the first digit of n */
    idx_t first = n;
    while (first >= 10) {
        first /= 10;
    }

    /* find the last digit of n */
    idx_t last = n % 10;

    /* concatenate (as a two‑digit number) */
    return (seq_t)first * (seq_t)10 + (seq_t)last;
}
