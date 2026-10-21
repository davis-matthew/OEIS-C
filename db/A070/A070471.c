/*
 * A070471  a(n) = n^3 mod 5.
 * https://oeis.org/A070471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 5
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo 5 to keep values tiny */
    idx_t r = n % 5;
    /* Compute r^3 and then take modulo 5, all in small integers */
    seq_t res = ((seq_t)(r * r * r)) % (seq_t)5;
    return res;
}
