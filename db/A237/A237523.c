/*
 * A237523  a(n) = |{0 < k < n/2: phi(k*(n-k)) + 1 is a square}|, where phi(.)
 *          is Euler's totient function.
 * https://oeis.org/A237523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n/2 : phi(k*(n-k)) + 1 is a square}|
 * Range:     u8 n <= 23, u16 n <= 4071, u32 n <= 4071, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    /* The allowed k satisfy 0 < k < n/2.
       For integer arithmetic this is equivalent to k <= (n-1)/2. */
    idx_t lim = (n - 1) / 2;
    seq_t count = 0;

    for (idx_t k = 1; k <= lim; ++k)
    {
        idx_t prod = k * (n - k);          /* k*(n-k)  (fits in idx_t) */
        seq_t ph   = phi(prod);             /* Euler's totient */
        seq_t val  = ph + 1;                /* phi(k*(n-k)) + 1   */
        seq_t s    = sqrt_floor(val);       /* floor(sqrt(val))   */

        if (s * s == val)
            ++count;
    }

    return count;
}
