/*
 * A098992  Number of permutations of [n] with exactly 2 descents which avoid
 *          the pattern 1324.
 * https://oeis.org/A098992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(-540 + 476*n + 469*n^2 - 490*n^3 + 70*n^4 + 14*n^5 +
 *            n^6) / 5040
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 78, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 1 or 2 the value is 0 because of the (n-2) factor. */
    if (n < 3)
        return 0;

    /* Use an unsigned 128‑bit intermediate to avoid overflow for n up to ~10⁴. */
    __uint128_t N   = n;          /* cast idx_t -> 128‑bit */
    __uint128_t n2  = N * N;      /* n^2 */
    __uint128_t n3  = n2 * N;     /* n^3 */
    __uint128_t n4  = n3 * N;     /* n^4 */

    /* Compute R(n) = n^4 + 17*n^3 + 119*n^2 - 167*n - 270 */
    __uint128_t R = n4
        + (__uint128_t)17 * n3
        + (__uint128_t)119 * n2;
    __uint128_t sub = (__uint128_t)167 * N + 270;
    R -= sub;                     /* safe subtraction for n ≥ 3 */

    /* Numerator: n*(n-1)*(n-2)*R(n) */
    __uint128_t num = N * (N - 1) * (N - 2) * R;

    /* Division by 5040 is exact for all n. */
    return (seq_t)(num / 5040);
}
