/*
 * A070606  n^5 mod 22.
 * https://oeis.org/A070606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 22
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 22 first; value fits comfortably in any seq_t size */
    seq_t x = (seq_t)(n % 22);
    seq_t r = x;
    /* Compute x^5 mod 22 efficiently, keeping intermediate values small */
    for (int i = 1; i < 5; ++i)
        r = (r * x) % 22;
    return r;
}
