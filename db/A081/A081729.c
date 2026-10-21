/*
 * A081729  Expansion of Sum(k>=0, x^(2^k)) + 1/(1+x). First differences of
 *          A007456 (gossip sequence) for n>1.
 * https://oeis.org/A081729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=0; For n>1: a(n)=2 if n is a power of 2, -1 if n is
 *            odd, 1 otherwise
 * Range:     i8 n <= 8892, i16 n <= 8892, i32 n <= 8892, i64 n <= 8892,
 *            i128 n <= 8892, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    /* power of 2 test uses bitwise AND; works for n>1 */
    if ((n & (n - 1)) == 0) return (seq_t)2;
    if ((n & 1ULL) != 0)     return (seq_t)(-1);
    return (seq_t)1;
}
