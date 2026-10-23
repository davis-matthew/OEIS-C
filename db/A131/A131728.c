/*
 * A131728  a(4n) = n, a(4n+1) = 2n+1, a(4n+2) = n+1, a(4n+3) = 0.
 * https://oeis.org/A131728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4n) = n, a(4n+1) = 2n+1, a(4n+2) = n+1, a(4n+3) = 0
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n >> 2;      /* n / 4  */
    switch (n & 3) {       /* n % 4  */
        case 0: return (seq_t)q;          /* a(4n) = n/4 */
        case 1: return (seq_t)((seq_t)2 * q + 1); /* a(4n+1) = 2*(n/4)+1 */
        case 2: return (seq_t)(q + 1);    /* a(4n+2) = (n/4)+1 */
        default: return 0;                /* a(4n+3) = 0 */
    }
}
