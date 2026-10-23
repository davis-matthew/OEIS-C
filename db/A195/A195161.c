/*
 * A195161  Multiples of 8 and odd numbers interleaved.
 * https://oeis.org/A195161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2n) = 8n, a(2n+1) = 2n+1
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For even indices use the even formula,
       for odd indices use the odd formula.   */
    if ((n & 1) == 0) {
        /* n is even: a(n) = 8 * (n/2) */
        return ((seq_t)(n >> 1)) * 8;
    } else {
        /* n is odd: a(n) = 2 * ((n-1)/2) + 1 */
        return ((seq_t)(n >> 1)) * 2 + 1;
    }
}
