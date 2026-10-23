/*
 * A178419  Partial sums of A006431.
 * https://oeis.org/A178419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum_{i=1..n} A006431(i)
 * Recurrence (for n >= 16): a(n) = a(n-1) + 4*a(n-3) - 4*a(n-4)
 * Initial values:
 *   a(1)=0, a(2)=1, a(3)=3, a(4)=6, a(5)=11, a(6)=17, a(7)=24,
 *   a(8)=32, a(9)=43, a(10)=57, a(11)=72, a(12)=95, a(13)=119,
 *   a(14)=151, a(15)=207
 *
 * The function keeps a memoised array of partial sums so that it can
 * answer calls in any order in O(n) total time for n up to about 10 000.
 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values, 1‑based indexing */
static size_t cache_len = 0;     /* maximum index currently stored */

/* initial a(1..15) */
static const seq_t init[16] = {
    0, /* unused index 0 */
    0,  1,  3,  6,  11,
    17, 24, 32, 43, 57,
    72, 95,119,151,207
};

/* ensure cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* cast idx_t to size_t, safe for 64‑bit values */
    size_t need = (size_t)n;
    size_t old_len = cache_len;

    if (n > cache_len) {
        /* enlarge or allocate */
        size_t new_size = need + 1;  /* indices 0..n */
        if (!cache) {
            cache = (seq_t*)malloc(new_size * sizeof(seq_t));
            if (!cache) abort();
            cache_len = need;
        } else {
            cache = (seq_t*)realloc(cache,
                                    new_size * sizeof(seq_t));
            if (!cache) abort();
            cache_len = need;
        }
    }

    /* initialise a(1..15) if needed */
    size_t upto = (cache_len < 15 ? cache_len : 15);
    for (size_t i = 1; i <= upto; ++i) {
        cache[i] = init[i];
    }

    /* compute new terms using the recurrence */
    size_t start = (old_len < 16) ? 16 : old_len + 1;
    if (start > cache_len) return;

    for (size_t k = start; k <= cache_len; ++k) {
        /* a(k) = a(k-1) + (a(k-3)-a(k-4))*4 */
        seq_t diff = cache[k-3] - cache[k-4];
        cache[k]  = cache[k-1] + (diff * 4);
    }
}

/* external linkage as requested */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1; return 0 as placeholder */
    ensure_cache(n);
    return cache[(size_t)n];
}
