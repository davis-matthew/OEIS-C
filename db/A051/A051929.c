/*
 * A051929  Number of independent sets of vertices in graph K_4 X C_n (n > 2).
 * https://oeis.org/A051929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2) + a(n-3), a(0)=5, a(1)=1, a(2)=21
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence.  The array is grown as needed. */
static seq_t *seq_cache = NULL;
static idx_t seq_cache_n = 0;   /* largest n for which seq_cache[n] is valid */

/* Ensure the cache contains terms up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (seq_cache == NULL) {
        /* allocate space for first 3 terms */
        seq_cache = (seq_t*)malloc(3 * sizeof(seq_t));
        seq_cache[0] = (seq_t)5;
        seq_cache[1] = (seq_t)1;
        seq_cache[2] = (seq_t)21;
        seq_cache_n  = 2;
        if (n <= 2) return;
    }

    if (n <= seq_cache_n) return;

    /* Grow the array to hold terms up to n */
    seq_t *new_cache = (seq_t*)realloc(seq_cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) { /* allocation failed – abort */
        abort();
    }
    seq_cache = new_cache;

    /* Compute new terms using the recurrence */
    for (idx_t i = seq_cache_n + 1; i <= n; ++i) {
        seq_cache[i] = 3 * seq_cache[i - 1] + 5 * seq_cache[i - 2] + seq_cache[i - 3];
    }
    seq_cache_n = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
