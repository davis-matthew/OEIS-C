/*
 * A099548  Odd part of n modulo 7.
 * https://oeis.org/A099548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd_part(n) mod 7 (a(n) = A010876(n) mod 7)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the odd part of n by removing all factors of 2 */
    idx_t odd = n;
    while ((odd & 1) == 0)  /* while odd is even */
        odd >>= 1;          /* divide by 2 */
    return (seq_t)(odd % 7); /* mod 7 fits in idx_t, cast to seq_t */
}
