/*
 * A089265  a(1) = 0; thereafter a(2*n) = a(n) + 1, a(2*n+1) = 2*n.
 * https://oeis.org/A089265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0; a(2*n)=a(n)+1; a(2*n+1)=2*n
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count how many times we can divide n by 2; each division adds 1 to the result. */
    seq_t res = 0;
    while ((n & 1) == 0) {
        n >>= 1;
        res += 1;
    }
    /* n is now odd (or 1). For odd n>1, a(n)=n-1. For n==1, this yields 0, matching a(1). */
    return res + ((seq_t)n - 1);
}
