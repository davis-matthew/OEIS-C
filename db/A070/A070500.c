/*
 * A070500  a(n) = n^3 mod 38.
 * https://oeis.org/A070500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 mod 38
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since the modulus is tiny we reduce n modulo 38 first to keep
       intermediate values small and work with any seq_t implementation. */
    const seq_t mod = (seq_t)38;
    seq_t nm = (seq_t)(n % 38ULL);      /* n modulo 38 */
    seq_t tmp = (nm * nm) % mod;       /* nm^2 mod 38 */
    return (tmp * nm) % mod;           /* nm^3 mod 38 = n^3 mod 38 */
}
