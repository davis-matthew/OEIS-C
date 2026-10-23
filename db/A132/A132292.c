/*
 * A132292  Integers repeated 8 times: a(n) = floor((n-1)/8).
 * https://oeis.org/A132292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/8)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* safety for n=0, though offset is 1 */
        return 0;
    return ((seq_t)(n - 1)) / 8;
}
