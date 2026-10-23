/*
 * A122619  n_{2n}.
 * https://oeis.org/A122619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (2*floor(n/10)+1)*n - 10*floor(n/10)
 * Range:     u8 n <= 39, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 10;                 /* k = floor(n/10) */
    seq_t sn = (seq_t)n;
    seq_t sk = (seq_t)k;
    seq_t res = ((sk * (seq_t)2 + (seq_t)1) * sn) - (sk * (seq_t)10);
    return res;
}
