/*
 * A069982  Number of 4-gonal compositions of n into positive parts.
 * https://oeis.org/A069982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3-3*n^2-23*n+3*(13+(n^2-7*n+11)*(-1)^n))/24 for n ≥
 *            4; a(0)=a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 15, u16 n <= 93, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 4) return (seq_t)0;
    /* use signed 64‑bit intermediate arithmetic */
    long long nn = (long long)n;
    long long term1 = 2 * nn * nn * nn;       /* 2*n^3 */
    long long term2 = 3 * nn * nn;            /* 3*n^2 */
    long long term3 = 23 * nn;                /* 23*n */
    long long s = (nn * nn - 7 * nn + 11);    /* n^2-7*n+11 */
    if (n & 1) s = -s;                       /* multiply by (-1)^n */
    long long numerator = term1 - term2 - term3 + 3 * (13 + s);
    return (seq_t)(numerator / 24);
}
