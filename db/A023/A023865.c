/*
 * A023865  a(n) = 1*t(n) + 2*t(n-1) + ... + k*t(n+1-k), where
 *          k=floor((n+1)/2) and t(n)=2*n-1 (odd numbers).
 * https://oeis.org/A023865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (8*n^3+24*n^2+10*n-3-3*(2*n^2+2*n-1)*(-1)^n)/48
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 2951, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t N2  = N * N;
    seq_t N3  = N2 * N;
    seq_t part3 = 2 * N2 + 2 * N - 1;          /* 2*n^2 + 2*n - 1 */
    seq_t t1   = 8 * N3 + 24 * N2 + 10 * N - 3;
    seq_t numer;
    if ((n & 1ULL) == 0)                       /* n even: subtract */
        numer = t1 - 3 * part3;
    else                                       /* n odd : add */
        numer = t1 + 3 * part3;
    return numer / 48;
}
