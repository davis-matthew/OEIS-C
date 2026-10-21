/*
 * A239057  Sum of the parts in the partitions of 4n into 4 parts with
 *          smallest part equal to 1 minus the number of these partitions.
 * https://oeis.org/A239057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n-1) * floor( ((4*n-1)^2 + 6) / 12 ) Equivalent: sum
 *            over all 4-part partitions of 4*n with smallest part 1 minus the
 *            number of such partitions
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 930, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute m = 4*n - 1 as seq_t to avoid overflow */
    seq_t m = (seq_t)n * 4 - 1;
    /* compute p3 = floor((m^2 + 6)/12)  - number of partitions of m into 3 parts */
    seq_t m2 = m * m;
    seq_t p3 = (m2 + 6) / 12;
    return m * p3;
}
