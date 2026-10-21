/*
 * A070630  a(n) = n^5 mod 47.
 * https://oeis.org/A070630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 47
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 47 first */
    seq_t n_mod = (seq_t)n % 47;
    seq_t r = 1;
    /* Compute n_mod^5 by repeated multiplication */
    for (int i = 0; i < 5; ++i) {
        r *= n_mod;
        r %= 47;
    }
    return r;
}
