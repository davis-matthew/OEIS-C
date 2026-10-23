/*
 * A048771  Partial sums of A048695.
 * https://oeis.org/A048771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(0)=1, a(1)=9, a(2)=26, a(n) = 3*a(n-1) - a(n-2) - a(n-3) (n ≥
 *            3)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th term of OEIS A048771.
 * Works with arbitrary‑precision seq_t (e.g. C++ class) or
 * fixed‑width unsigned integers; uses only integer arithmetic.
 * Memoisation ensures each term is computed once per program run. */
#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* memoised values of the sequence */
static size_t cache_len = 0;      /* number of values currently stored */
static size_t cache_cap = 0;      /* allocated capacity of seq_cache */

/* Ensure the cache is large enough to hold at least (n+1) entries. */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap == 0) new_cap = 1;
        if (new_cap < required) new_cap = required;
        seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialise base values once. */
    if (cache_len == 0) {
        ensure_cache(2);               /* at least space for a(0), a(1), a(2) */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)9;
        seq_cache[2] = (seq_t)26;
        cache_len = 3;
    }

    /* If necessary, extend cache up to index n. */
    if ((size_t)n >= cache_len) {
        ensure_cache(n);
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* Apply recurrence: a(i) = 3*a(i-1) - a(i-2) - a(i-3) */
            seq_t res = seq_cache[i - 1];
            res = res * (seq_t)3;
            res = res - seq_cache[i - 2];
            res = res - seq_cache[i - 3];
            seq_cache[i] = res;
        }
        cache_len = (size_t)n + 1;
    }

    return seq_cache[(size_t)n];
}
