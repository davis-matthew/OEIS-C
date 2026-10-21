/*
 * A066106  a(2*n) = (2*n)*(2*n+2); a(2*n+1) = 4*n + 4.
 * https://oeis.org/A066106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2*n) = (2*n)*(2*n+2); a(2*n+1) = 4*n + 4
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0)            /* even index: n = 2*k */
    {
        seq_t x = (seq_t)n;      /* cast index to seq_t first */
        return x * (x + 2);      /* x*(x+2) = (2*k)*(2*k+2) */
    }
    else                          /* odd index: n = 2*k+1 */
    {
        seq_t x = (seq_t)n;
        return 2 * (x + 1);      /* 2*(n+1) = 4*k+4 */
    }
}
