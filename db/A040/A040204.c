/*
 * A040204  Continued fraction for sqrt(219).
 * https://oeis.org/A040204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 14;
 * For n > 0 the sequence is multiplicative:
 *   - a(2)        = 3
 *   - a(2^e)      = 28 for e >= 2
 *   - a(p^e)      = 1 for every odd prime p
 *
 * Thus, for all n > 0:
 *   a(n) = 1                     if n is odd
 *   a(n) = 3                     if n = 2 * m with m odd
 *   a(n) = 28                    if n is divisible by 4
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)14;
    if ((n & 1) != 0) return (seq_t)1;  /* odd numbers */
    /* n is even, count powers of 2 */
    idx_t m = n;
    int e2 = 0;
    while ((m & 1) == 0) {
        e2++;
        m >>= 1;
    }
    if (e2 == 1) return (seq_t)3;    /* n = 2 * odd */
    return (seq_t)28;                /* n divisible by 4 */
}
