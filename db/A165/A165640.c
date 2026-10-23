/*
 * A165640  Number of distinct multisets of n integers, each of which is -2,
 *          +1, or +3, such that the sum of the members of each multiset is 3.
 * https://oeis.org/A165640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(4*(n+4)/5) - floor(2*(n+4)/3)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)(n + (idx_t)4);
    seq_t v1 = (seq_t)(4 * t / (seq_t)5);
    seq_t v2 = (seq_t)(2 * t / (seq_t)3);
    return v1 - v2;
}
