/*
 * A070444  a(n) = n^2 mod 22.
 * https://oeis.org/A070444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 22
 * Range:     u8 n <= 3000, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 22 first to avoid overflow, then square. */
    const seq_t MOD = (seq_t)22;
    seq_t t = (seq_t)(n % 22);
    return (t * t) % MOD;
}
