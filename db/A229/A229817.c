/*
 * A229817  Even bisection gives sequence a itself, n->a(2*(2*n+k)-1) gives
 *          k-th differences of a for k=1..2 with a(n)=n for n<2.
 * https://oeis.org/A229817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen
 * Formula:   a(0) = 0 a(1) = 1 a(2*n) = a(n) a(4*n+1) = a(n+1) - a(n)
 *            a(4*n+3) = a(n+2) - 2*a(n+1) + a(n)
 * Range:     i8 n <= 5458, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0ull) return (seq_t)0;
    if (n == 1ull) return (seq_t)1;

    /* even index */
    if ((n & 1ull) == 0ull)
        return a(n >> 1);

    /* odd index: 4*k+1 or 4*k+3 */
    if ((n & 3ull) == 1ull) {            /* 4*k+1 */
        idx_t k = (n - 1ull) >> 2;
        return a(k + 1) - a(k);
    } else {                              /* 4*k+3 */
        idx_t k = (n - 3ull) >> 2;
        return a(k + 2) - (seq_t)2 * a(k + 1) + a(k);
    }
}
