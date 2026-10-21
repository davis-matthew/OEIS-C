/*
 * A082410  a(1)=0. Thereafter, the sequence is constructed using the rule:
 *          for any k >= 0, if a(1), a(2), ..., a(2^k+1) are known, the next
 *          2^k terms are given as follows: a(2^k+1+i) = 1 - a(2^k+1-i) for 1
 *          <= i <= 2^k.
 * https://oeis.org/A082410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(4*n+2) = 1; a(4*n+4) = 0; a(2*n+1) = a(n+1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the trivial base case */
    if (n <= 1)
        return 0;

    /* even indices are resolved directly */
    if ((n & 1) == 0)                    /* n = 2k  */
        return ((n >> 1) & 1);           /* a(2k) = 0 if k even, 1 if k odd */

    /* odd indices: apply the recurrence a(2*m+1) = a(m+1) until an even n is reached */
    do {
        n = (n >> 1) + 1;                /* n = (n+1)/2 for odd n, without overflow */
    } while ((n & 1) != 0 && n > 1);

    /* now n is even, answer derived from n/2's parity */
    return ((n >> 1) & 1);
}
