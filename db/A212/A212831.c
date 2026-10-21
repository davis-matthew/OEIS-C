/*
 * A212831  a(4*n) = 2*n, a(2*n+1) = 2*n+1, a(4*n+2) = 2*n+2.
 * https://oeis.org/A212831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4*n) = 2*n; a(2*n+1) = 2*n+1; a(4*n+2) = 2*n+2
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n & 3;   /* n mod 4 */
    seq_t sn = (seq_t)n;
    switch (r)
    {
        case 0: return (sn / 4) * 2;          /* a(4*k) = 2*k */
        case 1: return sn;                   /* a(4*k+1) = 4*k+1 (= n) */
        case 2: return (sn / 2) + 1;         /* a(4*k+2) = 2*k+2 (= n/2+1) */
        case 3: return sn;                   /* a(4*k+3) = 4*k+3 (= n) */
    }
    /* unreachable, but required for completeness */
    return 0;
}
