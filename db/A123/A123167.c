/*
 * A123167  Continued fraction for
 *          c=sqrt(2)*(exp(sqrt(2))+1)/(exp(sqrt(2))-1). a(2*n-1) = 8*n-6,
 *          a(2*n) = 4*n-1.
 * https://oeis.org/A123167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(n) = 8*((n+1)/2)-6 for odd n, a(n) = 4*(n/2)-1 for even n
 *            (offset 1: a(1) = 2, a(2) = 3, …)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* guard against calls with n < 1 – behaviour undefined for offset, but allowed */
    if (n < 1) return 0;

    if (n & 1U) {               /* odd index: n = 2*k-1 => k = (n+1)/2 */
        idx_t k = (n + 1) >> 1; /* integer division by 2 */
        return (seq_t)8 * (seq_t)k - (seq_t)6;
    } else {                    /* even index: n = 2*k => k = n/2 */
        idx_t k = n >> 1;
        return (seq_t)4 * (seq_t)k - (seq_t)1;
    }
}
