/*
 * A070706  a(n) = n^7 mod 25.
 * https://oeis.org/A070706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 25
 * Range:     u8 n <= 344, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 25 first */
    seq_t x = (seq_t)n % (seq_t)25;
    seq_t res = 1;
    /* Compute x^7 mod 25 by repeated multiplication */
    for (int i = 0; i < 7; ++i) {
        res = (res * x) % (seq_t)25;
    }
    return res;
}
