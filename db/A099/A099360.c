/*
 * A099360  a(n) = (2^n + 1)^4 - 2.
 * https://oeis.org/A099360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n + 1)^4 - 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow = 1;
    /* compute 2^n by repeated multiplication by 2 */
    for (idx_t i = 0; i < n; ++i) {
        pow *= 2;
    }
    seq_t base = pow + 1;
    seq_t tmp  = base * base;     /* (2^n + 1)^2 */
    seq_t res  = tmp * tmp;       /* (2^n + 1)^4 */
    res -= 2;
    return res;
}
