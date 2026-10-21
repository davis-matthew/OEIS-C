/*
 * A082204  Begin with a 1, then place the smallest (as far as possible
 *          distinct) digits, such that, beginning from the n-th term, n terms
 *          form a palindrome.
 * https://oeis.org/A082204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = 3 if n mod 3 = 0, otherwise a(n) = 2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)1;
    }
    return (n % (idx_t)3 == (idx_t)0) ? (seq_t)3 : (seq_t)2;
}
