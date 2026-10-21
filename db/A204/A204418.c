/*
 * A204418  Periodic sequence 1,0,1,..., arranged in a triangle.
 * https://oeis.org/A204418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (n mod 3 == 1) ? 0 : 1 (periodic sequence 1,0,1)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)((n % 3ULL) == 1ULL ? 0 : 1);
}
