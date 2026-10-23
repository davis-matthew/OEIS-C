/*
 * A130658  Period 4: repeat [1, 1, 2, 2].
 * https://oeis.org/A130658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) is 1 for n mod 4 = 0 or 1, otherwise 2 (periodic [1,1,2,2])
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n mod 4 using bitwise AND (4 == 1<<2) */
    idx_t r = n & 3;          /* n % 4 */
    return (r < 2) ? (seq_t)1 : (seq_t)2;
}
