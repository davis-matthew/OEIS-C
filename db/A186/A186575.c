/*
 * A186575  Expansion of (1 + 2*x + 6*x^2)/(1 - x - x^2 - 2*x^3) in powers of
 *          x.
 * https://oeis.org/A186575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) Initial values: a(0)=1,
 *            a(1)=3, a(2)=10
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation uses a small memoised cache that grows on demand.
    Preconditions: seq_t supports + and assignment. */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;  /* array of previously‑computed terms   */
static idx_t cache_size = 0; /* number of valid entries in cache      */

/* Grow the cache so that indices 0 … n are valid.  The old data are
   copied into the new buffer; any new entries are zeroed. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;                    /* not needed */
    size_t new_size = (size_t)n + 1;               /* number of elements */
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) abort();                       /* out of memory */

    if (cache)                                      /* copy existing data */
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));

    for (idx_t i = cache_size; i < (idx_t)new_size; ++i)   /* init new */
        new_cache[i] = 0;

    free(cache);
    cache = new_cache;
    cache_size = (idx_t)new_size;
}

/* Initialise the three known starting terms.  This routine is safe to
   be called repeatedly; it performs the work only once. */
static void init_cache(void)
{
    if (cache_size == 0) {
        ensure_cache(2);          /* need indices 0 … 2 inclusive */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)10;
    }
}

/* Public function: return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    init_cache();          /* guarantees cache[0…2] are defined   */

    idx_t start = cache_size;   /* first index that is not yet known */
    ensure_cache(n);            /* guarantee space for 0 … n          */

    for (idx_t i = start; i <= n; ++i) {
        seq_t term = cache[i-1] + cache[i-2] + (cache[i-3] + cache[i-3]);
        cache[i] = term;
    }

    return cache[n];
}
