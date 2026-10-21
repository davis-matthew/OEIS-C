/*
 * A070629  a(n) = n^5 mod 46.
 * https://oeis.org/A070629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 46
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 46;
    /* Reduce n modulo MOD first to keep numbers small */
    seq_t m = (seq_t)n % MOD;

    /* Compute m^5 modulo MOD */
    seq_t res = m;          /* m^1 */
    res = (res * m) % MOD;  /* m^2 */
    res = (res * m) % MOD;  /* m^3 */
    res = (res * m) % MOD;  /* m^4 */
    res = (res * m) % MOD;  /* m^5 */

    return res;
}
