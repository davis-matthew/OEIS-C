/*
 * A070455  a(n) = n^2 mod 33.
 * https://oeis.org/A070455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 33
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t MOD = 33;
    idx_t r = n % MOD;              /* reduce n modulo 33 first */
    unsigned int prod = (unsigned int)r * (unsigned int)r; /* safe intermediate */
    return (seq_t)(prod % MOD);     /* result fits in seq_t */
}
