/*
 * A181404  Total number of positive integers below 10^n requiring 8 positive
 *          cubes in their representation as sum of cubes.
 * https://oeis.org/A181404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A181404: Number of positive integers below 10^n requiring exactly 8 positive cubes.
   Formula: a(1)=0, a(2)=3, a(n)=15 for n>2. */
seq_t a(idx_t n)
{
    if (n == 1ULL)
        return 0;
    if (n == 2ULL)
        return 3;
    return 15;
}
