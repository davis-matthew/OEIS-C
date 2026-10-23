/*
 * A106781  Primes with digit sum = 52.
 * https://oeis.org/A106781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th prime whose decimal digit sum is 52, offset 1
 *            (so a(1) is the smallest such prime)
 * Range:     u8 none, u16 none, u32 n <= 1800, u64 n <= 1800, u128 n <= 1800,
 *            big any n
 * Helpers:   digit_sum_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Helper functions supplied by the build. */
#include "../utils/is_prime.h"
#include "../utils/digit_sum_uint64.h"

/* The function required by the framework. */
seq_t a(idx_t n)
{
    /* Static state for memoisation. */
    static seq_t *primes   = NULL; /* array of found primes */
    static size_t  cap     = 0;     /* allocation capacity     */
    static size_t  count   = 0;     /* number of primes stored  */
    static uint64_t next   = 2;     /* next integer to test    */

    /* Ensure we have at least n primes. */
    while ((idx_t)count < n)
    {
        /* Find the next prime with digit sum 52. */
        uint64_t cand = next;

        while (true)
        {
            if (is_prime(cand) && digit_sum_uint64(cand) == 52)
                break;
            ++cand;
        }

        /* Store the prime. */
        if (count == cap)
        {
            size_t new_cap = (cap == 0) ? 64 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (tmp == NULL) /* allocation failed: abort (shouldn't happen). */
                abort();
            primes = tmp;
            cap = new_cap;
        }
        primes[count++] = (seq_t)cand;

        /* Advance the next candidate. */
        next = cand + 1;
    }

    return primes[n - 1];
}
