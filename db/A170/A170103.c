/*
 * A170103  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * For n ≥ 1: * a(n) = 44 * Σ_{k=1}^{min(36,n)} a(n–k) –
 *            990 * a(n–37) + C(n) * where C(0) = 1, * C(n) = 2 for 1 ≤ n ≤
 *            36, * C(37) = 1, * C(n) = 0 otherwise. * (This recurrence
 *            follows from the rational generating function * given in the
 *            OEIS entry.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t *cache      = NULL;   /* memoised values of the sequence */
static size_t cache_cap  = 0;       /* current allocated size */

/* Return the n‑th term of the sequence A170103 (offset 0). */
seq_t a(idx_t n)
{
    /* Expand the cache if needed. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp)
            abort();   /* out of memory */
        cache = tmp;

        /* Compute all missing terms up to n. */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0) {                /* a(0) = 1 */
                cache[i] = 1;
                continue;
            }

            /* Sum of the previous 1 … min(36,i) terms. */
            seq_t s = 0;
            size_t upto = (i <= 36 ? i : 36);
            for (size_t k = 1; k <= upto; ++k)
                s += cache[i - k];

            seq_t term = mul_small(s, 44);  /* 44 * sum */

            if (i >= 37)
                term -= mul_small(cache[i - 37], 990); /* –990*a(n–37) */

            if (i <= 36)
                term += 2;          /* +2 for 1 ≤ i ≤ 36 */
            else if (i == 37)
                term += 1;          /* +1 for i = 37 */

            cache[i] = term;
        }
        cache_cap = new_cap;
    }
    return cache[(size_t)n];
}
