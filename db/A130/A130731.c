/*
 * A130731  Period 4: repeat [1, 2, 0, 0].
 * https://oeis.org/A130731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + 1) mod 4) mod 3, with offset 0 (a(0)=1)
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n+1 modulo 4 and then reduce modulo 3 to get the periodic pattern
       1, 2, 0, 0 repeated forever. All operations stay within small integer ranges,
       so the result fits into seq_t for any size of seq_t. */
    idx_t r = (n + 1) % 4;
    return (seq_t)(r % 3);
}
