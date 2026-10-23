/*
 * A047391  Numbers that are congruent to {1, 3, 5} mod 7.
 * https://oeis.org/A047391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*⌊(n-1)/3⌋ + 2*((n-1) mod 3) + 1
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the sequence starts at a(1) = 1, so n >= 1 is assumed */
    idx_t m   = n - 1;          /* (n-1) fits in idx_t */
    idx_t k   = m / 3;          /* integer division */
    idx_t rem = m % 3;          /* remainder (0,1,2) */

    /* compute the value in the wide type seq_t */
    return (seq_t)7 * (seq_t)k + (seq_t)2 * (seq_t)rem + (seq_t)1;
}
