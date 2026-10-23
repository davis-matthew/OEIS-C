/*
 * A171522  Denominator of 1/n^2-1/(n+2)^2.
 * https://oeis.org/A171522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = -((-5+3*(-1)^n)*n^2*(n+2)^2)/8
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: a(0) = 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Use signed 64‑bit arithmetic for the intermediate steps.
       For the given limits (n up to about 10⁴), all intermediate
       values safely fit in int64_t. */
    int64_t n64   = (int64_t)n;                       /* convert n to signed */
    int64_t prod  = n64 * (n64 + 2);                  /* n*(n+2)       */
    int64_t prod2 = prod * prod;                      /* (n*(n+2))²    */

    /* Compute (-5 + 3*(-1)^n).  (-1)^n is 1 for even n, -1 for odd n. */
    int64_t parity = (n % 2 == 0) ? 1 : -1;
    int64_t factor = -5 + 3 * parity;                 /* factor is negative */

    /* numerator = factor * prod2 (negative) */
    int64_t numer = factor * prod2;

    /* result = -numer / 8  (positive) */
    uint64_t result = (uint64_t)(-numer / 8);

    return (seq_t)result;
}
