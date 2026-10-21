/*
 * A050104  a(n) = floor(a(n-1)/4) if this is not among 0, a(1), ..., a(n-2);
 *          otherwise a(n) = 3*a(n-1).
 * https://oeis.org/A050104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 677, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050104:
   a(1) = 1;
   For n > 1:
       let c = ⌊a(n‑1)/4⌋;
       if c == 0 or c ∈ {a(1), …, a(n‑2)} then
           a(n) = 3·a(n‑1);
       else
           a(n) = c. */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache – sufficient for n up to ~10 000 (adjust if needed). */
#define CACHE_CAP 200000

static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;

/* Ensure cache has computed entries up to index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= (idx_t)cache_len) return;

    /* Compute terms from cache_len+1 to n */
    for (idx_t i = cache_len; i < n; ++i)
    {
        if (i == 0) /* a(1) */
        {
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t prev = cache[i - 1];
        seq_t cand = prev / (seq_t)4;   /* floor division */
        bool used = false;

        if (cand == (seq_t)0)
            used = true;
        else
        {
            /* Search earlier terms for cand */
            for (idx_t j = 0; j < i - 1; ++j)
                if (cache[j] == cand)
                {
                    used = true;
                    break;
                }
        }
        cache[i] = used ? prev * (seq_t)3 : cand;
    }
    cache_len = (size_t)n;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
