/*
 * A134301  Periodic sequence (0, 2, 6, 2, 0).
 * https://oeis.org/A134301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1) mod 10
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce modulo 10 before multiplication to avoid overflow in any seq_t size. */
    seq_t n_mod  = (seq_t)(n % 10);
    seq_t np1_mod = (seq_t)((n + 1) % 10);
    return (n_mod * np1_mod) % 10;
}
