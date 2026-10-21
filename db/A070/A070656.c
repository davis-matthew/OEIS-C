/*
 * A070656  a(n) = n^6 mod 33.
 * https://oeis.org/A070656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 33
 * Range:     u8 n <= 274, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 33;
    seq_t r = (seq_t)n % MOD;        /* reduce n modulo 33 */
    seq_t ans = 1;
    for (int i = 0; i < 6; ++i)
        ans = (ans * r) % MOD;       /* compute r^6 mod 33 */
    return ans;
}
