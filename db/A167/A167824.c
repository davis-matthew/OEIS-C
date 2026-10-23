/*
 * A167824  Subsequence of A167709 whose indices are congruent to 3 mod 5,
 *          i.e., a(n) = A167709(5*n+3).
 * https://oeis.org/A167824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 340*a(n+1) - a(n) a(0) = 24, a(1) = 8175
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)24;
    if (n == 1) return (seq_t)8175;

    seq_t prev = (seq_t)24;
    seq_t curr = (seq_t)8175;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = (seq_t)340 * curr - prev;
        prev = curr;
        curr = next;
    }

    return curr;
}
