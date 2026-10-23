/*
 * A166654  Totally multiplicative sequence with a(p) = 5p-1 for prime p.
 * https://oeis.org/A166654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} (5p-1)^e (multiplicative, n≥1)
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 1535, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"

/* Cached values for any order of calls */
static seq_t *cache = NULL;     /* dynamic array, 1‑based */
static idx_t cache_size = 0;    /* highest index cached so far, 0 if none */

/* Return the n‑th term of A166654 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n<1 */

    /* Ensure cache is large enough.  Index n is always 1‑based. */
    if (cache == NULL || n > cache_size) {
        /* new size: indices 0..n  inclusive (0 is unused) */
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failed – silence */

        /* Preserve existing data */
        if (cache == NULL)
            tmp[0] = 0;          /* unused slot for index 0 */

        cache = tmp;

        /* Compute terms up to n (ignore already cached ones). */
        idx_t start = (cache_size == 0) ? 1 : cache_size + 1;
        for (idx_t i = start; i <= n; ++i) {
            uint64_t m = (uint64_t)i;

            /* Factorise m.  The helper expects arrays of distinct primes
               and their exponents.  The caller supplies enough space. */
            uint64_t primes[64];
            uint64_t exps[64];
            size_t cnt = 0;
            factor_u64(m, primes, exps, &cnt);

            seq_t val = 1;
            for (size_t j = 0; j < cnt; ++j) {
                uint64_t p = primes[j];
                uint64_t e = exps[j];

                /* Base = 5p-1 */
                seq_t base = (seq_t)5 * (seq_t)p - (seq_t)1;

                /* Compute base^e multiplicatively.  e ≤ 64 so the loop is
                   small and no overflow beyond seq_t capacity. */
                seq_t term = 1;
                for (uint64_t k = 0; k < e; ++k)
                    term *= base;

                val *= term;
            }
            cache[i] = val;
        }

        cache_size = n;
    }

    return cache[n];
}
