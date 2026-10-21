/*
 * A209074  Number of 5-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero and avoiding the pattern z z+1 z+2.
 * https://oeis.org/A209074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-6) -
 *            2*a(n-7) + 3*a(n-8) - 3*a(n-9) + a(n-10) Initial values: a(1)=8,
 *            a(2)=70, a(3)=276, a(4)=766, a(5)=1722, a(6)=3378, a(7)=6006,
 *            a(8)=9928, a(9)=15514, a(10)=23180
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* dynamically grown cache of terms */
static size_t cache_size = 0;        /* number of terms currently stored */

/* static const array of the first 10 terms */
static const seq_t init_terms[10] = {
    (seq_t)8,   (seq_t)70,  (seq_t)276,  (seq_t)766,
    (seq_t)1722,(seq_t)3378,(seq_t)6006, (seq_t)9928,
    (seq_t)15514,(seq_t)23180
};

static void ensure_initialization(void)
{
    /* if the cache is empty, allocate 10 elements and copy the known
       initial values.  This function is idempotent. */
    if (cache_size > 0)
        return;

    cache = (seq_t *)realloc(NULL, 10 * sizeof(seq_t));
    if (!cache) { /* allocation failure – the caller will get 0. */
        cache_size = 0;
        return;
    }
    for (size_t i = 0; i < 10; ++i)
        cache[i] = init_terms[i];
    cache_size = 10;
}

static void grow_cache(idx_t n)
{
    /* grow the cache to hold at least n terms (0‑based index n-1) */
    if ((idx_t)cache_size >= n)
        return;
    size_t new_size = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* retain the old cache; subsequent calls will try again.
           The caller will receive 0. */
        return;
    }
    cache = new_cache;
    /* compute missing terms starting from the current size up to n-1 */
    for (size_t i = cache_size; i < n; ++i) {
        /* We already have the first 10 terms in the cache.  For i>=10
           use the linear recurrence. */
        if (i < 10) {
            /* nothing to do – the initial terms are already set */
            continue;
        }
        unsigned __int128 pos = 0;
        unsigned __int128 neg = 0;

        pos += (unsigned __int128)3 * (unsigned __int128)cache[i-1];
        pos += (unsigned __int128)2 * (unsigned __int128)cache[i-3];
        pos += (unsigned __int128)2 * (unsigned __int128)cache[i-6];
        pos += (unsigned __int128)3 * (unsigned __int128)cache[i-8];
        pos +=                     (unsigned __int128)cache[i-10];

        neg += (unsigned __int128)3 * (unsigned __int128)cache[i-2];
        neg += (unsigned __int128)2 * (unsigned __int128)cache[i-4];
        neg += (unsigned __int128)2 * (unsigned __int128)cache[i-7];
        neg += (unsigned __int128)3 * (unsigned __int128)cache[i-9];

        cache[i] = (seq_t)(pos - neg);
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* as a defensive measure; the function is defined only for n>=1 */

    /* handle n <= 10 directly */
    if (n <= 10) {
        return init_terms[(size_t)n - 1];
    }

    /* initialize cache once */
    ensure_initialization();
    if (cache_size == 0)
        return (seq_t)0; /* allocation failed */

    /* grow cache to hold term n */
    grow_cache(n);
    if (cache_size < (size_t)n)
        return (seq_t)0; /* allocation failed during growth */

    return cache[(size_t)n - 1];
}
