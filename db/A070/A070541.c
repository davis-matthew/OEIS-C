/*
 * A070541  a(n) = n^4 mod 22.
 * https://oeis.org/A070541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 22
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)22;
    /* reduce the index first to avoid large intermediate results */
    seq_t x = (seq_t)(n % 22);          /* x ∈ [0,21] */
    seq_t r = (x * x) % MOD;            /* x^2 mod 22 */
    r = (r * x) % MOD;                  /* x^3 mod 22 */
    r = (r * x) % MOD;                  /* x^4 mod 22 */
    return r;
}
