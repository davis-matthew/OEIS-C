/*
 * A142691  Primes congruent to 43 mod 57.
 * https://oeis.org/A142691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 182, u32 n <= 109, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A142691: primes congruent to 43 (mod 57).
    a(1) = 43
    a(n) is computed by testing successive candidates of the form 57*k + 43
    and keeping those for which is_prime_big reports a prime.
*/

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cache up to a comfortably large number of terms.
   The sequence grows slowly, so 200000 slots are far more than enough
   for n up to 10000. */
static seq_t cache[200000];
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n < 1 */
        return 0;

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Start from the last found prime or from 43 if none yet */
    seq_t candidate = (cache_len ? cache[cache_len - 1] + (seq_t)57 : (seq_t)43);

    /* Generate primes until we have at least n terms */
    while (cache_len < n) {
        if (is_prime_big(candidate))
            cache[cache_len++] = candidate;
        candidate += (seq_t)57;
    }

    return cache[n - 1];
}
