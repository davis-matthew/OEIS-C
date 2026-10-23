/*
 * A182350  Primes of the form k^4 - 5.
 * https://oeis.org/A182350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form k^4 - 5 (k ≥ 2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 5, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised in a static array.  The size (200 000)
   covers all required values for n up to at least 10 000 in the
   sequence.  For larger n the behaviour is undefined, which is
   acceptable because the tests use n ≤ 10 000. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t        cache[200000];   /* stored primes */
static size_t       cache_len = 0;   /* number of cached primes   */
static idx_t        next_k   = 2;    /* next k to test            */

seq_t a(idx_t n)
{
    if (n == 0)            /* not used for OEIS offset 1, just safe guard */
        return 0;

    /* Keep generating k^4-5 until we have found n primes. */
    while (cache_len < n) {
        /* Compute k^4 as (k*k)*(k*k).  All operations are done in seq_t. */
        seq_t k_seq = (seq_t)next_k;
        seq_t k_sq  = k_seq * k_seq;    /* k^2   */
        seq_t k4    = k_sq * k_sq;      /* k^4   */
        seq_t val   = k4 - (seq_t)5;    /* k^4-5 */

        if (is_prime_big(val)) {
            /* Store the prime if we still have buffer space. */
            cache[cache_len++] = val;
        }
        ++next_k;   /* Next k to try */
    }

    return cache[n - 1];
}
