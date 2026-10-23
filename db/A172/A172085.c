/*
 * A172085  a(n) = n*(27*n^3 + 22*n^2 - 21*n - 16)/12.
 * https://oeis.org/A172085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (n+1) * (27*n^2 - 5*n - 16) / 12
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 207, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalent to a(n) = n*(27*n^3 + 22*n^2 - 21*n - 16)/12) */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    seq_t N  = (seq_t)n;
    seq_t N1 = N + 1;

    /* Compute 27*n^2 - 5*n - 16 using only unsigned arithmetic. */
    seq_t term = 27 * N * N;   /* 27*n^2   */
    term += (seq_t)(-5) * N;   /* subtract 5*n */
    term -= 16;                /* subtract 16 */

    seq_t temp = N * N1;       /* n*(n+1)   */
    temp = temp * term;        /* * (27*n^2 - 5*n - 16) */
    temp /= 12;                /* division is exact */

    return temp;
}
