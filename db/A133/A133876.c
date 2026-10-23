/*
 * A133876  n modulo 6 repeated 6 times.
 * https://oeis.org/A133876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + floor(n/6)) mod 6
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the integer division n/6, which is floor(n/6). */
    idx_t block = n / 6ull;
    /* Shift by +1 and take modulo 6.  The result is 0..5. */
    return (seq_t)((block + 1ull) % 6ull);
}
