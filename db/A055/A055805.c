/*
 * A055805  a(n) = T(n,n-5), array T as in A055801.
 * https://oeis.org/A055805
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n^5 -45*n^4 +450*n^3 -2070*n^2 +4873*n -3585)
 *            +5*(-1)^n*(n^4 -34*n^3 +446*n^2 -2741*n +6861))/7680 for n > 5,
 *            with offset 5 (a(5)=a(6)=a(7)=1)
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 444, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return 0;          /* undefined for n < 5 */
    if (n <= 7) return (seq_t)1;  /* a(5)=a(6)=a(7)=1 */

    /* Use signed 128‑bit arithmetic for intermediate values. */
    __int128 N  = ( __int128 ) n;
    __int128 N2 = N * N;
    __int128 N3 = N2 * N;
    __int128 N4 = N3 * N;
    __int128 N5 = N4 * N;

    /* Polynomial part P1 = 2*n^5 -45*n^4 +450*n^3 -2070*n^2 +4873*n -3585 */
    __int128 P1 = 2 * N5
                - 45 * N4
                + 450 * N3
                - 2070 * N2
                + 4873 * N
                - 3585;

    /* Polynomial part P2 = n^4 -34*n^3 +446*n^2 -2741*n +6861 */
    __int128 P2 = N4
                - 34 * N3
                + 446 * N2
                - 2741 * N
                + 6861;

    /* Sign factor (-1)^n */
    long long sign = (n & 1ULL) ? -1LL : 1LL;

    __int128 numerator = P1 + 5 * sign * P2;
    __int128 result    = numerator / 7680;   /* exact division */

    return (seq_t) result;
}
