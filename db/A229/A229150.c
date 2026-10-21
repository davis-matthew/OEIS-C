/*
 * A229150  a(n) = n^7*(9*n+7)/2.
 * https://oeis.org/A229150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7*(9*n+7)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 13, u64 n <= 212, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = n;
    if ((nn & 1) == 0) {
        /* n is even: divide n^7 by 2 first */
        seq_t half = nn >> 1;                      /* n/2 */
        seq_t n2   = nn * nn;                      /* n^2 */
        seq_t n4   = n2 * n2;                      /* n^4 */
        seq_t n6   = n4 * n2;                      /* n^6 */
        seq_t prod = half * n6;                    /* n^7/2 */
        prod *= (9 * nn + 7);                      /* * (9n+7) */
        return prod;
    } else {
        /* n is odd: (9n+7) is even, divide it by 2 first */
        seq_t t   = (9 * nn + 7) >> 1;             /* (9n+7)/2 */
        seq_t n2  = nn * nn;                      /* n^2 */
        seq_t n4  = n2 * n2;                      /* n^4 */
        seq_t n6  = n4 * n2;                      /* n^6 */
        seq_t prod = n6 * nn;                     /* n^7 */
        prod *= t;                                /* * ((9n+7)/2) */
        return prod;
    }
}
