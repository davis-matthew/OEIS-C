/*
 * A071329  Largest prime q such that q - prime(n) <= n.
 * https://oeis.org/A071329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime ≤ n + prime(n)
 * Range:     u8 n <= 46, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here prime(n) is the nth prime (A000040) and the function returns the
 * largest prime not exceeding n + prime(n) (A007917 of that argument). */

#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* nth prime */
    idx_t p = get_prime(n);

    /* bound = n + prime(n) */
    idx_t bound = n + p;

    /* If bound itself is prime that's the answer */
    if (is_prime(bound))
        return (seq_t)bound;

    /* Otherwise search downward for the first prime < bound */
    for (idx_t m = bound - 1; m >= 2; --m) {
        if (is_prime(m))
            return (seq_t)m;
    }
    /* Should never reach here for n ≥ 1 */
    return (seq_t)2;
}
