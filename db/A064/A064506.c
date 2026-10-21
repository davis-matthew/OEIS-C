/*
 * A064506  a(n) = Max { k | k*(k+1)/2 <= n*(n+1)/2 - k*(k+1)/2 }.
 * https://oeis.org/A064506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((sqrt(2*n*(n+1) + 1) - 1) / 2) (for n ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* Outside the official offset we return a harmless value. */
    if (n == 0) {
        return 0;
    }

    /* 2*n*(n+1) + 1 is computed in 128‑bit to avoid overflow for the
       64‑bit build.  The value fits into unsigned __int128 for all n
       used in the tests (up to about 10 000). */
    unsigned __int128 tmp = (unsigned __int128)n * (n + 1);
    tmp = tmp << 1;          /* tmp = 2 * n * (n + 1)          */
    tmp += 1;                /* tmp = 2*n*(n+1) + 1            */

    /* Convert to the generic seq_t type.  The cast invokes the
       appropriate constructor for the concrete integer type. */
    seq_t t = (seq_t)tmp;

    /* Compute the integer square root of t. */
    seq_t s = isqrt_seq(t);          /* floor(sqrt(t))                 */

    /* The closed form solution of the problem. */
    seq_t result = (s - 1) / 2;
    return result;
}
