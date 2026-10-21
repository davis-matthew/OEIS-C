/*
 * A064264  a(n) = 19*n mod 30.
 * https://oeis.org/A064264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (19 * n) mod 30
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo 30 first to avoid overflow on 64‑bit seq_t */
    idx_t n_mod = n % 30;
    return ((seq_t)n_mod * (seq_t)19) % (seq_t)30;
}
