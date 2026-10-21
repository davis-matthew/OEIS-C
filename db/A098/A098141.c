/*
 * A098141  a(1)=1, then if n not a square a(n)=a(n-1) + n, if n a square then
 *          a(n)= a(n-1) - n.
 * https://oeis.org/A098141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)/2 -
 *            floor(sqrt(n))*(floor(sqrt(n))+1)*(2*floor(sqrt(n))+1)/3 + 2 *
 *            (Derived from a(1)=1; for n>1: a(n)=a(n-1)+n if n is not a
 *            square, otherwise a(n)=a(n-1)-n
 * Range:     u8 n <= 26, u16 n <= 374, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (idx_t)1 << 32;           /* sqrt(UINT64_MAX) < 2^32   */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid <= n / mid)          /* safe comparison avoiding overflow */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, but safety for n<1 */

    idx_t m = isqrt_u64(n);                         /* floor(sqrt(n)) */

    seq_t N = (seq_t)n;                           /* use seq_t for all arithmetic */
    seq_t T = N * (N + 1) / 2;                     /* sum 1..n */

    seq_t M = (seq_t)m;
    seq_t U = M * (M + 1) * (2 * M + 1) / 3;       /* sum of squares 1^2..m^2 */

    return T - U + 2;                              /* final result */
}
