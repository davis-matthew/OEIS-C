/*
 * A070717  a(n) = n^7 mod 36.
 * https://oeis.org/A070717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 36
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo 36 first – this keeps all intermediate values small. */
    idx_t m = n % 36;          /* m < 36 fits in uint64_t */
    seq_t x = (seq_t)m;        /* base for exponentiation */

    /* Compute x^7 mod 36 by repeated multiplication. */
    /* Since x < 36, all products stay below 36^2 = 1296, so no overflow
       even on the smallest builds. */
    for (int i = 0; i < 6; ++i) /* (i = 0..5) multiplies 6 times → power 7 */
        x = (x * (seq_t)m) % 36;

    return x;
}
