/*
 * A152893  Periodic sequence [3, 3, 0, 0, 4] of period 5.
 * https://oeis.org/A152893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n-2)^3 - (n-2)^2) mod 5
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence with period 5: 3, 3, 0, 0, 4 */
seq_t a(idx_t n)
{
    /* All calculations are done in idx_t to avoid overflow */
    const idx_t MOD = 5;
    /* Reduce n modulo 5 */
    idx_t n_mod = n % MOD;
    /* k = (n - 2) modulo 5, safely handling underflow */
    idx_t k_mod = (n_mod + MOD - 2) % MOD;    /* 0..4 */
    /* Compute k^2 modulo 5 */
    idx_t k2 = (k_mod * k_mod) % MOD;
    /* Compute (k - 1) modulo 5 */
    idx_t k_minus1 = (k_mod + MOD - 1) % MOD;
    /* a(n) = k^2 * (k - 1) modulo 5 */
    idx_t val = (k2 * k_minus1) % MOD;
    return (seq_t)val;
}
