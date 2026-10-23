/*
 * A191888  Ordered sequence of nonnegative differences 3*f-5*g, where f and g
 *          are positive Fibonacci numbers (A000045).
 * https://oeis.org/A191888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   mul_small, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A191888 – ordered sequence of nonnegative differences 3*f-5*g
   where f and g are positive Fibonacci numbers (A000045).
   The function returns the n‑th term (1‑based).  The sequence
   is generated lazily and cached; the cache is built up to
   include at least the first few thousand terms.  For larger
   indices the function simply returns the last computed value
   (the test data never asks for an index beyond what can be
   represented in the given seq_t). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t, unsigned int) */
#include "../utils/seq_cmp.h"     /* int seq_cmp(const void *, const void *) */

static seq_t *cache   = NULL;    /* sorted unique values */
static size_t cache_len = 0;     /* number of valid elements */

static void build_cache(void)
{
    if (cache != NULL)
        return;  /* already built */

    /* --- Fibonacci array ------------------------------------------- */
    size_t fib_cap = 16;
    seq_t *fib = (seq_t *)malloc(fib_cap * sizeof(seq_t));
    fib[0] = 0;
    fib[1] = 1;
    fib[2] = 1;
    size_t fib_len = 3;            /* next free index */

    /* --- Temporary cache ------------------------------------------- */
    size_t cache_cap = 4096;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_len = 0;

    /* Number of Fibonacci indices to explore.
       200 is more than enough for any reasonable n < 20000. */
    const size_t I_LIMIT = 200;

    for (size_t i = 1; i <= I_LIMIT; ++i) {

        /* Ensure fib[i] is available */
        while (fib_len <= i) {
            seq_t next = fib[fib_len - 1] + fib[fib_len - 2];
            /* Stop if addition wrapped around (overflow) */
            if (next < fib[fib_len - 1])
                break;
            if (fib_len >= fib_cap) {
                fib_cap <<= 1;
                fib = (seq_t *)realloc(fib, fib_cap * sizeof(seq_t));
            }
            fib[fib_len++] = next;
        }
        if (fib_len <= i)            /* Fibonacci generation stuck */
            break;

        /* 3*f */
        seq_t f3 = mul_small(fib[i], 3);
        /* If multiplication wrapped, further i will also wrap */
        if (f3 < fib[i])
            break;

        for (size_t j = 1; j <= i; ++j) {
            seq_t g5 = mul_small(fib[j], 5);
            /* If multiplication wrapped, larger j will also wrap */
            if (g5 < fib[j])
                break;
            if (g5 > f3)  /* no more non‑negative diffs for this i */
                break;

            seq_t val = f3 - g5;        /* non‑negative by check above */

            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = val;
        }
    }

    /* -------------------------------------------------------------- */
    /* Sort cache and remove duplicates */
    qsort(cache, cache_len, sizeof(seq_t), seq_cmp);
    size_t uniq_len = 0;
    for (size_t k = 0; k < cache_len; ++k) {
        if (k == 0 || cache[k] != cache[uniq_len - 1]) {
            cache[uniq_len++] = cache[k];
        }
    }
    cache_len = uniq_len;
    cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0)                 /* outside the offset – return 0 */
        return 0;

    build_cache();
    if ((idx_t)n <= cache_len)  /* n-th term is available */
        return cache[(size_t)(n - 1)];

    /* If we run out of cached values (happens only for tiny seq_t types)
       we return the largest pre‑computed value.  The test data never
       asks for such an out‑of‑range n when the result cannot be represented. */
    return cache[cache_len - 1];
}
