/*
 * A165983  Period 16: repeat [1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 4, 1, 1, 1,
 *          4].
 * https://oeis.org/A165983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1 for n mod 16 in {0,1,2,4,5,6,8,9,10,12,13,14}; 2 for n
 *            mod 16 in {3,7}; 4 for n mod 16 in {11,15}
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 16. */
    idx_t r = n % (idx_t)16;
    if (r == (idx_t)3 || r == (idx_t)7)
        return (seq_t)2;
    if (r == (idx_t)11 || r == (idx_t)15)
        return (seq_t)4;
    return (seq_t)1;
}
