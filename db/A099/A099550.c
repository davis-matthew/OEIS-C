/*
 * A099550  Odd part of n modulo 9.
 * https://oeis.org/A099550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (odd part of n) mod 9 Where odd part of n is n with all
 *            factors of 2 removed
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Remove powers of two from n to get its odd part. */
    idx_t odd = n;
    while ((odd & 1) == 0)
        odd >>= 1;

    /* Return the odd part modulo 9.  The result fits in 0..8. */
    return (seq_t)(odd % 9);
}
