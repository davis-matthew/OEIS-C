/*
 * A057354  a(n) = floor(2*n/5).
 * https://oeis.org/A057354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*n/5)
 * Range:     u8 n <= 639, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(2*n/5) without overflow:
       Let q = n/5, r = n%5. Then
       floor(2*n/5) = 2*q + floor(2*r/5).  */
    idx_t q = n / 5;
    idx_t r = n % 5;
    seq_t res = (seq_t)q * 2 + ((seq_t)r * 2) / 5;
    return res;
}
