/*
 * A070508  a(n) = n^3 mod 46.
 * https://oeis.org/A070508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 46
 * Range:     u8 n <= 272, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t s = (seq_t)n;

    /* Compute s^3 modulo 46, keeping intermediate values small */
    seq_t t = (s * s) % 46;   /* s^2 mod 46  (t < 46) */
    seq_t r = (t * s) % 46;   /* (s^2 mod 46) * s mod 46  (r < 46) */

    return r;
}
