/*
 * A211164  Number of compositions of n with at most one odd part.
 * https://oeis.org/A211164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = 1; For even n > 0: a(n) = 2^((n-2)/2) For odd n
 *            ≥ 3: a(n) = ((n+3)/2) * 2^((n-3)/2)
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 58, u64 n <= 120,
 *            u128 n <= 246, big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Only uses integer arithmetic and the utilities pow2() and seq_mul() */

#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Handle the special small indices directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    if ((n & 1) == 0) {          /* n even */
        idx_t exp = (n - 2) / 2; /* exponent is non‑negative */
        return pow2(exp);
    } else {                     /* n odd, n ≥ 3 */
        idx_t exp   = (n - 3) / 2;
        seq_t p     = pow2(exp);
        seq_t fact  = (seq_t)((n + 3) / 2);
        return seq_mul(p, fact);
    }
}
