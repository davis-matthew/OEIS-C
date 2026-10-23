/*
 * A109422  Numbers n such that tau(n)/bigomega(n) is not an integer [tau(n)
 *          =number of divisors of n; bigomega(n)=number of prime divisors of
 *          n, counted with multiplicities].
 * https://oeis.org/A109422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer >1 such that tau(n)/bigomega(n) is NOT
 *            an integer
 * Range:     u8 n <= 54, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   factor_tau_omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iteratively test each integer starting at 2, factorise it,
   compute tau and bigomega, and collect those for which tau mod bigomega ≠ 0. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_tau_omega.h"
/* External types are defined by the build system: seq_t and idx_t */

/* Non‑static external function required by the test harness
   Return the n‑th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    /* Static cache for the sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;
    /* Next integer to test */
    static uint64_t next_candidate = 2;

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t tau, omega;
        factor_tau_omega(next_candidate, &tau, &omega);

        if (omega > 0 && tau % omega != 0) {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();        /* memory allocation failure */
                cache = tmp;
                cache_cap = (idx_t)new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    /* Return the desired element (1‑based index) */
    return cache[n - 1];
}
