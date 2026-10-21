/*
 * A079833  Define f(n) = n + tau(n), then a(n) = smallest prime of the type
 *          f(f(f...f(n)))).
 * https://oeis.org/A079833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the first prime that appears from iterating f(n) = n +
 *            τ(n) (apply f at least once)
 * Range:     u8 n <= 39, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   isPrimeGeneric, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/isqrt_seq.h"

/* Compute τ(n) for an arbitrary seq_t value */
static seq_t tau_seq(seq_t n)
{
    seq_t t = n;
    seq_t result = 1;
    seq_t limit = isqrt_seq(t);           /* floor(√t) */
    for (seq_t p = 2; p <= limit; ++p) {
        if (t % p == 0) {
            seq_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            result *= (e + 1);
            /* t decreased ⇒ recompute limit to shorten loop */
            limit = isqrt_seq(t);
        }
    }
    if (t > 1)          /* final prime factor > √original t */
        result *= 2;
    return result;
}

/* Sequence a(n) from OEIS A079833 */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;         /* start value */
    do {
        cur += tau_seq(cur);      /* apply f once */
    } while (!isPrimeGeneric(cur));
    return cur;
}
