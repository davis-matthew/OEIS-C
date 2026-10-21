/*
 * A099903  Sum of all matrix elements of n X n matrix M(i,j) = i^3+j^3, (i,j
 *          = 1..n). a(n) = n^3*(n+1)^2/2.
 * https://oeis.org/A099903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*(n+1)^2/2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    __uint128_t N = n;           /* promote to 128‑bit to avoid overflow */
    __uint128_t res = N * N * N; /* N^3 */
    res *= (N + 1);              /* * (n+1) */
    res *= (N + 1);              /* * (n+1) again */
    res /= 2;                    /* divide by 2 */
    return (seq_t)res;           /* cast back to seq_t (fits for the required range) */
}
