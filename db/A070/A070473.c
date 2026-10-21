/*
 * A070473  a(n) = n^3 mod 11.
 * https://oeis.org/A070473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 11
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 11 using the wider type to avoid overflow. */
    idx_t n_mod = n % 11;
    seq_t x   = (seq_t)n_mod;   /* x is now in [0,10] */
    seq_t y   = x * x;          /* y is in [0,100]    */
    y          = y * x;          /* y is in [0,1000]   */
    return y % (seq_t)11;        /* final result in [0,10] */
}
