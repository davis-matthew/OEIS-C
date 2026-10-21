/*
 * A215604  a(0)=0, a(n) = (n + a(floor(n/2))) mod 3.
 * https://oeis.org/A215604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + a(floor(n/2))) mod 3; a(0) = 0
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t half = a(n / 2);                /* recursive call on floor(n/2) */
    return ((seq_t)(n % 3) + half) % 3;   /* (n + a(floor(n/2))) mod 3 */
}
