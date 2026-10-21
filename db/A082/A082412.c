/*
 * A082412  a(n) = (2*8^n + 2^n)/3.
 * https://oeis.org/A082412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*8^n + 2^n)/3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t res  = (seq_t)1;       /* current a(i-1), start with a(0) */
    seq_t pow2 = (seq_t)1;       /* 2^0 */

    for (idx_t i = 1; i <= n; ++i) {
        pow2 <<= 1;              /* pow2 = 2^i   */
        res = res * (seq_t)8 - pow2; /* a(i) = 8*a(i-1) - 2^i */
    }
    return res;
}
