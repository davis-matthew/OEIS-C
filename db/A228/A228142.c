/*
 * A228142  Number of blocks in a Steiner system S(2, 5, A228141(n+1)).
 * https://oeis.org/A228142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n*n + n/2 if n is even * a(n) = (10*n*n - 11*n + 3)/2
 *            if n is odd * (for all n >= 1)
 * Range:     u8 n <= 6, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1, but guard for safety */

    /* Cast n to seq_t for arithmetic */
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;                 /* n^2 */

    if ((n & 1) == 0) {                 /* even n */
        seq_t half = (seq_t)(n / 2);     /* n/2 (exact integer) */
        return (seq_t)5 * sq + half;
    } else {                            /* odd n */
        seq_t numerator = (seq_t)10 * sq - (seq_t)11 * sn + (seq_t)3;
        return numerator / (seq_t)2;   /* division is exact */
    }
}
