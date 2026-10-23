/*
 * A133886  a(n) = binomial(n+6,n) mod 6.
 * https://oeis.org/A133886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6,6) mod 6
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is periodic with period 72, so reducing n keeps
       intermediate values small. */
    idx_t n_mod = n % 72ULL;

    seq_t res = 1;
    for (idx_t i = 1; i <= 6; ++i) {
        res = res * (n_mod + i);
        res = res / i;          /* exact division */
    }
    return res % 6;
}
