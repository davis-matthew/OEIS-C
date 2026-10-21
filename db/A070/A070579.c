/*
 * A070579  a(n) = n^4 mod 36.
 * https://oeis.org/A070579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4) mod 36
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n first, because n can be very large but 36 is tiny. */
    seq_t r = (seq_t)n % 36;
    /* Compute r^4 modulo 36.  r is in [0,35], so intermediate products fit easily. */
    seq_t r2 = (r * r) % 36;      /* r^2 mod 36 */
    seq_t r4 = (r2 * r2) % 36;    /* r^4 mod 36 */
    return r4;
}
