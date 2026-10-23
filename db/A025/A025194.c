/*
 * A025194  a(n) = [ 3rd elementary symmetric function of {sqrt(k)} ], k =
 *          1,2,...,n.
 * https://oeis.org/A025194
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sum_{1 <= i < j < k <= n} sqrt(i*j*k) )
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Efficient O(n) computation using recurrence:
 * a(k) = a(k-1) + sqrt(k) * T(k-1)
 * T(k) = sum_{1 <= i < j <= k} sqrt(i*j)
 *       = (S(k)^2 - k(k+1)/2) / 2
 * where S(k) = sum_{i=1}^k sqrt(i)  */
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;           /* offset is 3, values <3 are undefined */

    long double S = 0.0L;                  /* S(k) = sum sqrt(i) up to current k */
    long double T = 0.0L;                  /* T(k) = sum sqrt(i*j) for i<j <= current k */
    long double A = 0.0L;                  /* cumulative a(k) */

    for (idx_t k = 1; k <= n; ++k) {
        long double sqrtk = sqrtl((long double)k);

        /* add contribution of new triples containing k */
        if (k > 2) {
            A += sqrtk * T;                /* T already contains sum up to k-1 */
        }

        /* update T for next iteration (pairs with new element k) */
        T += sqrtk * S;                    /* S is sum up to k-1 */

        /* update S for next iteration (add sqrt(k)) */
        S += sqrtk;
    }

    /* Cast the result to seq_t, adding tiny epsilon to guard against
       rounding down caused by negative floating‑point error. */
    uint64_t res = (uint64_t)floorl(A + 1e-7L);
    return (seq_t)res;
}
