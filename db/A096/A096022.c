/*
 * A096022  Numbers that are congruent to {15, 27, 39, 51} mod 60.
 * https://oeis.org/A096022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*( n + floor((n-1)/4) ) + 3 (a(1)=15)
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure offset starting at 1 */
    if (n < 1)
        return 0;   /* undefined for n < 1, return 0 as fallback */

    /* k is the n-th positive integer not divisible by 5 */
    idx_t k = n + (n - 1) / 4;   /* integer division */

    /* Compute the term: a(n) = 12*k + 3 */
    return (seq_t)12 * (seq_t)k + (seq_t)3;
}
