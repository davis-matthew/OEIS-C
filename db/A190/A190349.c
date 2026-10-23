/*
 * A190349  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=sqrt(5/3), t=sqrt(3/5).
 * https://oeis.org/A190349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + floor(n*sqrt(5/3)) + floor(2*n/3)
 * Range:     u8 n <= 6, u16 n <= 1434, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the same integer type as the sequence values */
    seq_t N = (seq_t)n;

    /* Compute floor(n*sqrt(5/3)) by binary search on k:
       3*k^2 <= 5*N^2 < 3*(k+1)^2  <=>  k = floor(N*sqrt(5/3)) */
    seq_t low  = 0;
    seq_t high = 2 * N;                     /* sqrt(5/3) < 2  */
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;   /* ceil division to avoid infinite loop */
        seq_t lhs = 3 * mid * mid;           /* 3*mid^2  */
        seq_t rhs = 5 * N * N;               /* 5*N^2    */
        if (lhs <= rhs)
            low = mid;
        else
            high = mid - 1;
    }
    seq_t sqrt_term = low;                   /* floor(n*sqrt(5/3)) */

    /* floor(2*n/3) is just integer division */
    seq_t two_n_div3 = (2 * N) / 3;

    /* Final result */
    return 2 * N + sqrt_term + two_n_div3;
}
