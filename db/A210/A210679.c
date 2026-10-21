/*
 * A210679  Number of distinct prime factors <= 7 of n.
 * https://oeis.org/A210679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{p∈{2,3,5,7}} [p | n] (number of distinct prime
 *            factors ≤7 of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n % 2 == 0) count++;
    if (n % 3 == 0) count++;
    if (n % 5 == 0) count++;
    if (n % 7 == 0) count++;
    return count;
}
