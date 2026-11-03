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
 * Validated: standard, pass, 47 terms through n = 1668 (2026-11-03)
 * Helpers:   digit_sum_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Helper functions supplied by the build. */
#include "../utils/digit_sum_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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
            if (is_prime_u64(cand) && digit_sum_u64(cand) == 52)
                break;
            ++cand;
        }

        /* Store the prime. */
        if (count == cap)
        {
            size_t new_cap = (cap == 0) ? 64 : cap * 2;
            seq_t *tmp = mem_resize(seq_t, primes, new_cap);
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
