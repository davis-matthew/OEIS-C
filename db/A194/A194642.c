/*
 * A194642  Units' digits of the nonzero heptagonal numbers.
 * https://oeis.org/A194642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n*(5*n-3)/2) mod 10
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD20 = 20;
    const seq_t MOD10 = 10;
    /* Reduce n modulo 20 to avoid large intermediate values */
    seq_t n_mod = (seq_t)(n % MOD20);

    /* Compute (5*n - 3) modulo 20 */
    seq_t term = ((seq_t)5 * n_mod) % MOD20;
    term = (term + MOD20 - 3) % MOD20;   /* (5*n - 3) mod 20 */

    /* Compute numerator modulo 20: n_mod * term */
    seq_t num = (n_mod * term) % MOD20;     /* numerator is guaranteed even */

    /* Divide by 2 and take modulo 10 */
    seq_t res = (num / 2) % MOD10;
    return res;
}
