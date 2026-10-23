/*
 * A023260  Primes that remain prime through 2 iterations of function f(x) =
 *          8x + 1.
 * https://oeis.org/A023260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are primes p such that 8p+1 and 64p+9 are also primes
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, isPrimeGeneric, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(idx_t n) returns the n-th element of the sequence.
   It keeps a static cache of found terms and extends it lazily on
   demand.  The search proceeds by testing consecutive integers for
   primality via the build‑supplied helper functions */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static variables retain their values between calls */
    static size_t cache_size = 0;            /* number of terms cached     */
    static size_t cache_cap  = 0;            /* capacity of cache array    */
    static seq_t *cache        = NULL;       /* array of cached terms      */
    static idx_t last_checked  = 1;          /* last integer examined      */

    while (cache_size < n) {
        last_checked++;                     /* next integer to test       */
        if (!is_prime(last_checked))        /* skip non‑primes            */
            continue;

        /* cast to the build’s integer type for the arithmetic */
        seq_t p  = (seq_t)last_checked;
        seq_t f1 = (seq_t)8 * p + 1;
        seq_t f2 = (seq_t)64 * p + 9;

        if (isPrimeGeneric(f1) && isPrimeGeneric(f2)) {
            /* grow the cache if necessary */
            ensure_capacity(cache_size + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_size++] = p;
        }
    }
    return cache[n - 1];
}
