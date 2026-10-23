/*
 * A167176  a(n) = n^3 mod 9.
 * https://oeis.org/A167176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 9
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 9 first */
    seq_t n_mod9 = (seq_t)(n % 9);

    /* Compute n_mod9^2 modulo 9, then multiply by n_mod9 again */
    seq_t r = (n_mod9 * n_mod9) % 9;
    r = (r * n_mod9) % 9;

    return r;
}
