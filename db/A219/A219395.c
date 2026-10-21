/*
 * A219395  Numbers k such that 18*k+1 is a square.
 * https://oeis.org/A219395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m*(18*m+2), where m = (n-1)/2 for odd n m = -(n/2) for
 *            even n (m(1)=0)
 * Range:     u8 n <= 7, u16 n <= 121, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute signed m */
    long long m;
    if (n & 1ULL) {
        /* odd n: m = (n-1)/2 >= 0 */
        m = ((long long)n - 1LL) / 2LL;
    } else {
        /* even n: m = -(n/2) <= 0 */
        m = -((long long)n / 2LL);
    }

    /* absolute value of m */
    seq_t mm = (seq_t)(m >= 0 ? m : -m);

    /* compute 18*m*m + 2*m with care for sign */
    seq_t term1 = 18ULL * mm * mm;
    seq_t result = term1;
    seq_t add    = 2ULL * mm;

    if (m >= 0) {
        result += add;
    } else {
        result -= add;
    }

    return result;
}
