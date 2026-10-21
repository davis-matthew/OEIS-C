/*
 * A070569  a(n) = n^4 mod 26.
 * https://oeis.org/A070569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 26
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)26;
    seq_t r  = (seq_t)n % MOD;     /* n mod 26 */
    seq_t r2 = (r * r) % MOD;      /* (n mod 26)^2 mod 26 */
    seq_t r4 = (r2 * r2) % MOD;    /* (n mod 26)^4 mod 26 */
    return r4;
}
