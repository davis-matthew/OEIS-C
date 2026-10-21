/*
 * A070477  a(n) = n^3 mod 15.
 * https://oeis.org/A070477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n mod 15)^3 mod 15
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo 15 first to avoid large intermediate values. */
    seq_t t = (seq_t)n % 15;

    /* Compute the cube of the reduced value, continuing to apply the modulus. */
    seq_t cube = (t * t) % 15;
    cube = (cube * t) % 15;

    return cube;
}
