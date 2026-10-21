/*
 * A212572  Number of (w,x,y,z) with all terms in {1,...,n} and |w-x| <= |x-y|
 *          + |y-z|.
 * https://oeis.org/A212572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/48 * (38*n^4 + 20*n^3 - 32*n^2 - 2*(3*(-1)^n - 11)*n +
 *            3*((-1)^n - 1))
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 271, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* use signed 64-bit intermediate to avoid overflow */
    long long nn = n;
    long long n2 = nn * nn;          /* n^2 */
    long long n3 = n2 * nn;          /* n^3 */
    long long n4 = n3 * nn;          /* n^4 */

    long long inside;
    if ((n & 1ULL) == 0) {          /* n even, (-1)^n = 1 */
        inside = 38LL * n4 + 20LL * n3 - 32LL * n2 + 16LL * nn;
    } else {                        /* n odd, (-1)^n = -1 */
        inside = 38LL * n4 + 20LL * n3 - 32LL * n2 + 28LL * nn - 6LL;
    }

    /* divide by 48; result always integer */
    seq_t result = (seq_t)(inside / 48LL);
    return result;
}
