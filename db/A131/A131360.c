/*
 * A131360  a(4n) = a(4n+1) = 0, a(4n+2) = 2n, a(4n+3) = 2n+1.
 * https://oeis.org/A131360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4n)=0, a(4n+1)=0, a(4n+2)=2n, a(4n+3)=2n+1
 * Range:     u8 n <= 513, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 4 */
    switch (n & 3) {
    case 0:
    case 1:
        return 0;                /* a(4n) = 0, a(4n+1) = 0 */
    case 2: {
        /* a(4n+2) = 2n,  where n = floor(n/4) */
        seq_t m = (seq_t)(n >> 2);
        return m << 1;           /* 2 * m */
    }
    case 3: {
        /* a(4n+3) = 2n+1,  where n = floor(n/4) */
        seq_t m = (seq_t)(n >> 2);
        return (m << 1) | 1;     /* 2 * m + 1 */
    }
    }
    /* unreachable, but keep compiler happy */
    return 0;
}
