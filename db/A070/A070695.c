/*
 * A070695  a(n) = n^7 mod 13.
 * https://oeis.org/A070695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 13
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n mod 13 first (no overflow possible) */
    unsigned int r = (unsigned int)(n % 13u);

    /* Compute r^7 modulo 13 */
    unsigned int res = 1u;
    for (unsigned int i = 0; i < 7u; ++i)
        res = (res * r) % 13u;

    return (seq_t)res;
}
