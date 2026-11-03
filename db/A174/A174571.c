/*
 * A174571  a(4n)=n, a(4n+1)=4, a(4n+2)=1, a(4n+3)=4.
 * https://oeis.org/A174571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4n)=n, a(4n+1)=4, a(4n+2)=1, a(4n+3)=4
 * Range:     u8 n <= 1023, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 100 terms through n = 9320 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n & 3) {
        case 0:  return (seq_t)(n / 4);   /* a(4k) = k */
        case 1:  return (seq_t)4;        /* a(4k+1) = 4 */
        case 2:  return (seq_t)1;        /* a(4k+2) = 1 */
        case 3:  return (seq_t)4;        /* a(4k+3) = 4 */
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
