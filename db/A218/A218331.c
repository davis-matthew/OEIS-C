/*
 * A218331  Even, nonzero decagonal pyramidal numbers.
 * https://oeis.org/A218331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16*n-4*(-1)^n-1)*(4*n-(-1)^n+3)*(4*n-(-1)^n+1)/24
 * Range:     u8 none, u16 n <= 6, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset starts at 1 */
    seq_t n_t = (seq_t)n;
    seq_t term1, term2, term3;

    if ((n & 1) == 0) {            /* even n => (-1)^n = 1 */
        term1 = (seq_t)16 * n_t - 5;   /* 16*n - 5   */
        term2 = (seq_t)4 * n_t + 2;    /* 4*n + 2    */
        term3 = (seq_t)4 * n_t;        /* 4*n        */
    } else {                       /* odd n => (-1)^n = -1 */
        term1 = (seq_t)16 * n_t + 3;   /* 16*n + 3   */
        term2 = (seq_t)4 * n_t + 4;    /* 4*n + 4    */
        term3 = (seq_t)4 * n_t + 2;    /* 4*n + 2    */
    }

    seq_t prod = term1 * term2;
    prod = prod * term3;
    return prod / 24;
}
