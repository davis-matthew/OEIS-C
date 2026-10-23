/*
 * A103805  Primes p such that 2*p - 27 and 2*p + 27 are positive primes.
 * https://oeis.org/A103805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Search for primes p such that 2p-27 and 2p+27 are primes
 * Range:     u8 n <= 8, u16 n <= 339, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

static seq_t *primes_arr = NULL;   /* array of found primes p */
static size_t sz = 0;              /* number of primes stored */
static size_t cap = 0;             /* current capacity of primes_arr */
static idx_t last_checked = 1;     /* last candidate examined */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n = 0, but return 0 */

    while (sz < n) {
        uint64_t candidate = last_checked + 1;
        last_checked = candidate;

        if (!is_prime(candidate))
            continue;                                 /* not prime */

        uint64_t low  = 2 * candidate - 27;
        uint64_t high = 2 * candidate + 27;

        if (low < 2)            /* ensure positivity */
            continue;
        if (!is_prime(low) || !is_prime(high))
            continue;         /* both neighbours must be prime */

        /* store the candidate (fits in seq_t for the required n) */
        ensure_capacity(sz + 1, &primes_arr, &cap);
        primes_arr[sz++] = (seq_t)candidate;
    }

    return primes_arr[n - 1];
}
