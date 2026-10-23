/*
 * A126592  Sum of numbers less than or equal to n which are multiples of 3 or
 *          5.
 * https://oeis.org/A126592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sn(n,3) + sn(n,5) - sn(n,15) where sn(n,d) = (an(n,d) *
 *            (an(n,d)+d)) / (2*d) and an(n,d) = d * floor(n/d)
 * Range:     u8 n <= 32, u16 n <= 529, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* multiples of 3 */
    seq_t an3 = (seq_t)(3 * (n / 3));
    seq_t sn3 = (an3 * (an3 + 3)) / (6);

    /* multiples of 5 */
    seq_t an5 = (seq_t)(5 * (n / 5));
    seq_t sn5 = (an5 * (an5 + 5)) / (10);

    /* multiples of 15 (counted twice) */
    seq_t an15 = (seq_t)(15 * (n / 15));
    seq_t sn15 = (an15 * (an15 + 15)) / (30);

    /* inclusion–exclusion */
    return sn3 + sn5 - sn15;
}
