/*
 * A182480  Decimal expansion of 16000000/63.
 * https://oeis.org/A182480
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = pattern[(n-6) mod 6] where pattern = {2,5,3,9,6,8}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {2, 5, 3, 9, 6, 8};
    return pattern[(n - 6) % 6];
}
