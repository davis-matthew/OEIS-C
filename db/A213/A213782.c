/*
 * A213782  Principal diagonal of the convolution array A213781.
 * https://oeis.org/A213782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^3+33*n^2-14*n)/24 for n even a(n) =
 *            (8*n^3+33*n^2-20*n+3)/24 for n odd (equivalent to the closed
 *            form from the OEIS entry)
 * Range:     u8 n <= 8, u16 n <= 56, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute using 64‑bit signed arithmetic */
    long long nl = (long long)n;
    long long cube = nl * nl * nl;     /* n³  */
    long long square = nl * nl;        /* n²  */
    long long val;

    if ((n & 1ULL) == 0ULL) {          /* even n */
        val = (8LL * cube + 33LL * square - 14LL * nl) / 24LL;
    } else {                           /* odd n */
        val = (8LL * cube + 33LL * square - 20LL * nl + 3LL) / 24LL;
    }
    return (seq_t)val;
}
