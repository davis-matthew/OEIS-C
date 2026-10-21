/*
 * A201110  Primes of the form 2n^3+7.
 * https://oeis.org/A201110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2k^3+7 where k ≥ 0 and the value is prime; a(n) is the
 *            n‑th such prime. The OEIS lists a(1)=7, a(2)=23, …
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 164, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   ensure_capacity, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Static memoization of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of primes stored so far */
    static size_t cache_cap = 0;   /* capacity of the cache */
    static idx_t next_k = 0;       /* next integer k to test (k starts at 0) */

    if (n == 0) {
        return 0;                  /* OEIS offset starts at 1 */
    }

    /* Generate terms until we have at least n entries. */
    while (cache_len < (size_t)n) {
        seq_t k = (seq_t)next_k;
        seq_t f = ((seq_t)2 * k * k * k) + (seq_t)7;  /* 2k³+7 */

        if (isPrimeGeneric(f)) {
            /* Grow the cache if necessary. */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = f;
        }
        next_k++;                   /* advance to next candidate */
    }

    /* cache[0] corresponds to a(1). */
    return cache[n - 1];
}
