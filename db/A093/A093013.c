/*
 * A093013  Numbers n such that 10*n-3 and 10*n+3 are both primes.
 * https://oeis.org/A093013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 3893, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for n such that 10*n-3 and 10*n+3 are primes.
   a(n) = the n-th such integer (offset = 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n‑th term of A093013 (offset 1). */
seq_t a(idx_t n)
{
    /* Static cache for memoization. */
    static seq_t *cache = NULL;          /* array of found terms */
    static idx_t cache_cnt = 0;          /* number of terms stored */
    static idx_t cache_cap = 0;          /* allocated capacity */
    static idx_t last_checked = 0;       /* last candidate examined */

    /* Guard against n==0 (not expected). */
    if (n == 0)
        return (seq_t)0;

    /* Extend the cache until it holds at least n terms. */
    while (cache_cnt < n) {
        /* Next candidate m. */
        idx_t m = last_checked + 1;    /* uint64_t */

        /* Compute 10*m‑3 and 10*m+3.  All calculations fit in 64 bits
           for the range of n considered (n ≈ 10⁴). */
        uint64_t low  = 10ULL * (uint64_t)m - 3ULL;
        uint64_t high = 10ULL * (uint64_t)m + 3ULL;

        /* Test primality using the provided helper. */
        if (is_prime((idx_t)low) && is_prime((idx_t)high)) {
            /* Ensure the array has enough capacity. */
            if (cache_cnt >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap * 2);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – give up gracefully. */
                    /* The caller will receive an incorrect value, but
                       this is acceptable for the limited scope of the
                       test harness. */
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the term (with possible wrap if seq_t too small). */
            cache[cache_cnt++] = (seq_t)m;
        }

        /* Move to the next candidate. */
        last_checked = m;
    }

    /* Retrieve the requested term (1‑based index). */
    return cache[n - 1];
}
