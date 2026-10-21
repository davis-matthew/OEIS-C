/*
 * A210981  A062725 and positive terms of A051682 interleaved.
 * https://oeis.org/A210981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n^2+14*n-5+(6*n+5)*(-1)^n)/16
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    long long nl = (long long)n;
    long long term1 = 18LL*nl*nl + 14LL*nl - 5LL;
    long long term2 = ((6LL*nl + 5LL) * ((n & 1ULL) ? -1LL : 1LL));
    long long res   = (term1 + term2) / 16LL;
    return (seq_t)res;
}
