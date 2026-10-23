/*
 * A166925  Digital root of square of n-th triangular number.
 * https://oeis.org/A166925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, less, easy, nonn
 * Formula:   a(n) = digital_root( (n(n+1)/2)^2 )
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digital root of an integer x is x mod 9, with 0 replaced by 9 for x>0. */

seq_t a(idx_t n)
{
    /* Compute T_n = n(n+1)/2 modulo 9 without explicit division. */
    /* Since 2⁻¹ ≡ 5 (mod 9), T_n ≡ n(n+1)*5 (mod 9). */
    seq_t n_mod = (seq_t)(n % 9);
    seq_t np1_mod = (seq_t)((n + 1) % 9);
    seq_t t_mod = (n_mod * np1_mod) % 9;   /* n(n+1) mod 9           */
    t_mod = (t_mod * 5) % 9;               /* divide by 2 modulo 9    */

    /* Square the triangular number modulo 9. */
    seq_t sq_mod = (t_mod * t_mod) % 9;

    /* Convert 0 to 9 for positive numbers. */
    return (sq_mod == 0) ? 9 : sq_mod;
}
