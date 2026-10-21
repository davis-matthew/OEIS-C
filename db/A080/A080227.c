/*
 * A080227  a(n) = n*a(n-1) + (1/2)*(1+(-1)^n), a(0)=0.
 * https://oeis.org/A080227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*a(n-1) + (1/2)*(1+(-1)^n), a(0)=0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;          /* a(0) = 0 */
    for (idx_t i = 1; i <= n; ++i) {
        /* Multiply the previous term by the current index */
        seq_t prod = (seq_t)i * val;
        /* (1/2)*(1+(-1)^i) is 1 if i is even, otherwise 0 */
        seq_t add = ((i & 1) == 0) ? (seq_t)1 : (seq_t)0;
        val = prod + add;
    }
    return val;
}
