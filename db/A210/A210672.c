/*
 * A210672  a(0)=1; thereafter a(n) = 2*Sum_{k=1..n} binomial(2n,2k)*a(n-k).
 * https://oeis.org/A210672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 216 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A210672
 * a(0) = 1
 * a(n) = 2 * Σ_{k=1..n} binomial(2n, 2k) * a(n-k)   (n ≥ 1)
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

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

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Make sure the cache holds indices up to n. */
    ensure_cache(n, &cache, &cache_len);

    if (cache[0] == (seq_t)0)
        cache[0] = 1;                 /* a(0) = 1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] != (seq_t)0)
            continue;                 /* already computed */

        seq_t sum = 0;
        seq_t bin = 1;                /* C(2*i, 0) */

        for (idx_t k = 1; k <= i; ++k)
        {
            /* Update bin to C(2*i, 2k) from C(2*i, 2k-2) */
            idx_t num1 = (idx_t)(2 * i - 2 * k + 2);   /* 2i‑2k+2 */
            idx_t num2 = (idx_t)(2 * i - 2 * k + 1);   /* 2i‑2k+1 */
            idx_t den  = (idx_t)(2 * k * (2 * k - 1)); /* 2k*(2k‑1) */

            bin = mul_small(bin, (unsigned)num1);
            bin = mul_small(bin, (unsigned)num2);
            bin = bin / (seq_t)den;

            sum += bin * cache[i - k];
        }

        cache[i] = sum * 2;
    }

    return cache[n];
}
