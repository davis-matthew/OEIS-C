/*
 * A212986  Number of (w,x,y) with all terms in {0,...,n} and 2w = 3x+y.
 * https://oeis.org/A212986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of (w,x,y) in {0,…,n}³ with 2w = 3x + y Enumerated
 *            combinatorially: for each x, count admissible y with same parity
 *            as x and 3x + y ≤ 2n
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t two_n = 2 * n;          /* _will_ wrap if n > ULLONG_MAX/2, but that case is
                                     outside practical use_ */
    for (idx_t x = 0; x <= n; ++x) {
        idx_t thr = 3 * x;
        /* If 3x > 2n no y can satisfy 3x + y ≤ 2n */
        if (thr > two_n) continue;

        idx_t y_max = two_n - thr;
        if (y_max > n) y_max = n;
        idx_t parity = x & 1;

        if (y_max < parity) continue;

        idx_t half = (y_max - parity) >> 1;   /* floor((y_max - parity)/2) */
        seq_t count = (seq_t)(half + 1);
        res += count;
    }
    return res;
}
