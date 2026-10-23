/*
 * A160411  Number of cells turned "ON" at n-th stage of A160117.
 * https://oeis.org/A160411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 8 For n > 2: if n is even: a(n) = 12*n - 20 if
 *            n is odd: a(n) = 2*n - 2
 * Range:     u8 n <= 23, u16 n <= 5463, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)8;

    if ((n & 1) == 0) {            /* even n */
        seq_t prod = (seq_t)12 * (seq_t)n;  /* 12*n */
        return prod - (seq_t)20;           /* 12*n - 20 */
    } else {                       /* odd n */
        seq_t prod = (seq_t)2 * (seq_t)n;   /* 2*n */
        return prod - (seq_t)2;            /* 2*n - 2 */
    }
}
