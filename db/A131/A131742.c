/*
 * A131742  a(4n) = a(4n+1) = 0, a(4n+2) = 3n+1, a(4n+3) = 3n+2.
 * https://oeis.org/A131742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4n) = 0 a(4n+1) = 0 a(4n+2) = 3n + 1 a(4n+3) = 3n + 2 for n >=
 *            0, offset 0
 * Range:     u8 n <= 341, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n div 4 in the index type to avoid signed overflow */
    idx_t n_div4 = n >> 2;         /* floor(n/4) */
    unsigned int r = n & 3;        /* n mod 4 */

    switch (r) {
        case 0:   /* 4n */
        case 1:   /* 4n+1 */
            return 0;
        case 2:   /* 4n+2 */
        case 3:   /* 4n+3 */
        {
            /* Cast to seq_t once for arithmetic */
            seq_t m = (seq_t)n_div4;
            if (r == 2)
                return (seq_t)3 * m + (seq_t)1;
            else /* r == 3 */
                return (seq_t)3 * m + (seq_t)2;
        }
    }
    /* unreachable, but keep a fallback */
    return 0;
}
