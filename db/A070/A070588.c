/*
 * A070588  a(n) = n^4 mod 45.
 * https://oeis.org/A070588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 45
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type once, then work modulo 45. */
    seq_t x = (seq_t)n;
    seq_t r = x % (seq_t)45;          /* reduce modulo 45 first to keep values small */
    seq_t r2 = (r * r) % (seq_t)45;   /* r^2 mod 45 */
    seq_t r4 = (r2 * r2) % (seq_t)45; /* r^4 mod 45 */
    return r4;
}
