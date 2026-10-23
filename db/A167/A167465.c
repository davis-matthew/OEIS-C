/*
 * A167465  a(n) = n^5 mod 16.
 * https://oeis.org/A167465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 16
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n^5 mod 16 is 0 for even n, and equals n mod 16 for odd n */
    if ((n & (idx_t)1) == (idx_t)0)
        return (seq_t)0;
    else
        return (seq_t)(n % (idx_t)16);
}
