/*
 * A071238  a(n) = n*(n+1)*(2*n^2+1)/6.
 * https://oeis.org/A071238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n^2+1)/6
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 336, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a = n;                       /* a = n        */
    seq_t b = n + 1;                   /* b = n + 1    */
    seq_t n2 = n * n;                  /* n2 = n^2     */
    seq_t c = (seq_t)2 * n2 + 1;       /* c = 2*n^2 + 1 */

    /* Cancel a factor of 2 */
    if ((a & 1) == 0) {
        a >>= 1;               /* a /= 2      */
    } else if ((b & 1) == 0) {
        b >>= 1;               /* b /= 2      */
    } else {
        c >>= 1;               /* c /= 2      */
    }

    /* Cancel a factor of 3 */
    if (a % 3 == 0) {
        a /= 3;                /* a /= 3      */
    } else if (b % 3 == 0) {
        b /= 3;                /* b /= 3      */
    } else {
        c /= 3;                /* c /= 3      */
    }

    /* Multiply the remaining products */
    seq_t result = a * b;
    result *= c;

    return result;
}
