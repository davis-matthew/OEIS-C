/*
 * A187277  Let S denote the palindromes in the language {0,1,2,...,n-1}*;
 *          a(n) = number of words of length 4 in the language SS.
 * https://oeis.org/A187277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(2*n^2 + n - 2)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn   = (seq_t)n;
    seq_t term = (seq_t)2 * tn * tn + tn - (seq_t)2;
    return tn * term;
}
