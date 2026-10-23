/*
 * A164888  Smallest positive integer for each n such that the sequences a(n),
 *          a(n)+n, and a(n)+2n have no repeated terms.
 * https://oeis.org/A164888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer such that the three arithmetic
 *            progressions a(n)+k*n (k=0,1,2) contain no number already used
 *            by a(1)..a(n-1). This implementation builds the sequence
 *            incrementally, caching values for future calls. It works for any
 *            unsigned integral seq_t (including arbitrary-pre...
 * Range:     u8 n <= 113, u16 n <= 8357, u32 n <= 8357, u64 n <= 8357,
 *            u128 n <= 8357, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached values of a(n).  a_cache[i] holds a(i+1). */
static seq_t *a_cache = NULL;
static size_t cache_cap = 0;      /* allocated size of a_cache */
static size_t cache_len = 0;      /* number of values we have computed */

/* Boolean array marking all numbers that have already appeared in any
   of the three progressions up to the current n. */
static unsigned char *used = NULL; /* 0 = unused, non‑zero = used */
static size_t used_cap = 0;        /* allocated size of used */

/* Ensure the used array can address index 'need' (0‑based).  The array
   grows exponentially. */
static void grow_used(size_t need)
{
    if (need < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 64;
    while (new_cap <= need)
        new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (!tmp)
        abort();
    /* Initialise the newly allocated part. */
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* Ensure the sequence cache can hold at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    /* Initialise the newly allocated part. */
    memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    a_cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for any n >= 1.  Calls are memoised; the sequence is
   generated up to the largest requested n on demand. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1 – callers should use n>=1 */
        return 0;

    /* Generate values up to n if we haven't yet. */
    if ((size_t)cache_len < n) {
        grow_cache((size_t)n);

        for (size_t cur = cache_len + 1; cur <= (size_t)n; ++cur) {
            size_t nn = cur;        /* current index (1‑based) */
            size_t cand = 1;        /* candidate a(n) */

            /* Find the smallest unused candidate. */
            for (;;) {
                size_t need = cand + 2 * nn;
                grow_used(need);    /* ensure indices exist */
                if (!used[cand] && !used[cand + nn] && !used[cand + 2 * nn])
                    break;
                ++cand;
            }

            /* Store the result and mark the numbers as used. */
            seq_t val = (seq_t)cand;
            a_cache[cur - 1] = val;

            used[cand] = 1;
            used[cand + nn] = 1;
            used[cand + 2 * nn] = 1;
        }
        cache_len = n;
    }

    return a_cache[n - 1];
}
