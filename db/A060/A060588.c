/*
 * A060588  If the final two digits of n written in base 3 are the same then
 *          this digit, otherwise mod 3-sum of these two digits.
 * https://oeis.org/A060588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (-n - floor(n/3)) mod 3
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n and floor(n/3) are reduced modulo 3 first to keep values small */
    seq_t n_mod3 = (seq_t)(n % 3);                /* n mod 3  (0,1,2) */
    seq_t q_mod3 = (seq_t)((n / 3) % 3);          /* floor(n/3) mod 3 (0,1,2) */

    /* compute ( -n_mod3 ) mod 3  and ( -q_mod3 ) mod 3  safely */
    seq_t term1 = (n_mod3 == 0) ? (seq_t)0 : (seq_t)(3 - n_mod3);
    seq_t term2 = (q_mod3 == 0) ? (seq_t)0 : (seq_t)(3 - q_mod3);

    /* sum and reduce mod 3 */
    seq_t result = term1 + term2;
    result %= 3;
    return result;
}
