/*
 * A078752  Least m>n such that phi(m) >= 2*phi(n), where phi is Euler's
 *          totient function (A000010).
 * https://oeis.org/A078752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m > n such that φ(m) ≥ 2·φ(n)
 * Range:     u8 n <= 126, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The utility header defines the Euler totient function:
 *   seq_t phi(idx_t m);
 */
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Compute 2·φ(n), using a larger integer type to avoid overflow in very small seq_t builds. */
    unsigned long long target = (unsigned long long)phi(n) * 2ULL;

    /* Search for the first integer m > n with φ(m) ≥ target. */
    for (idx_t m = n + 1; ; ++m) {
        if ((unsigned long long)phi(m) >= target) {
            /* Return m cast to seq_t. */
            return (seq_t)m;
        }
    }
}
