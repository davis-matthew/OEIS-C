/*
 * A100722  Prime numbers whose binary representations are split into exactly
 *          five runs.
 * https://oeis.org/A100722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth prime whose binary representation splits into exactly
 *            five runs
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Count the number of runs of consecutive identical bits in x.
   An early exit returns if more than 5 runs are found. */
static inline int binary_runs(uint64_t x)
{
    if (x == 0) return 1;          /* binary 0 has one run of 0s */

    int runs  = 1;
    uint64_t prev = x & 1ULL;
    x >>= 1;
    while (x)
    {
        uint64_t cur = x & 1ULL;
        if (cur != prev)
        {
            ++runs;
            if (runs > 5) return runs;  /* early exit */
        }
        prev = cur;
        x >>= 1;
    }
    return runs;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    static uint64_t candidate = 2;

    if (n == 0)
        return 0; /* undefined for offset 1 */

    while (cache_size < n) {
        /* test current candidate */
        if (is_prime_u64(candidate) && binary_runs(candidate) == 5) {
            /* grow cache if needed */
            if (cache_size == cache_cap) {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)
                    abort(); /* memory allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)candidate;
        }

        /* advance candidate to next odd number */
        candidate = (candidate == 2) ? 3 : candidate + 2;
    }

    return cache[n - 1];
}
