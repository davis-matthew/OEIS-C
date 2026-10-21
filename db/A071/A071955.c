/*
 * A071955  a(n) = remainder when n is reduced mod reverse(n).
 * https://oeis.org/A071955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n mod reverse(n) (remainder when n is divided by its
 *            decimal reversal)
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute reverse of n in seq_t */
    seq_t rev = 0;
    idx_t temp = n;
    while (temp > 0) {
        rev = rev * 10 + (temp % 10);
        temp /= 10;
    }
    /* If n is palindromic, rev == n and remainder is 0 automatically */
    return ((seq_t)n) % rev;
}
