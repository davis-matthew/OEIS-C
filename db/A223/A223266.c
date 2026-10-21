/*
 * A223266  Rolling cube footprints: number of n X 5 0..5 arrays starting with
 *          0 where 0..5 label faces of a cube and every array movement to a
 *          horizontal, diagonal or antidiagonal neighbor moves across a
 *          corresponding cube edge.
 * https://oeis.org/A223266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 448*a(n-1) - 46080*a(n-2) + 491520*a(n-3) +
 *            37748736*a(n-4) - 637534208*a(n-5) + 2147483648*a(n-6) initial
 *            values: a(1)=256, a(2)=67584, a(3)=19726336, a(4)=5889851392,
 *            a(5)=1771674009600, a(6)=534392715870208
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t c448          = (seq_t)448;
static const seq_t c46080        = (seq_t)46080;
static const seq_t c491520       = (seq_t)491520;
static const seq_t c37748736     = (seq_t)37748736;
static const seq_t c637534208    = (seq_t)637534208;
static const seq_t c2147483648   = (seq_t)2147483648ULL;

static const seq_t init[6] = {
    (seq_t)256ULL,
    (seq_t)67584ULL,
    (seq_t)19726336ULL,
    (seq_t)5889851392ULL,
    (seq_t)1771674009600ULL,
    (seq_t)534392715870208ULL
};

static seq_t *cache = NULL;     /* 0‑based: cache[0] = a(1) */
static idx_t cache_size = 0;    /* number of stored terms */

static void
ensure_terms(idx_t required)
{
    if (required <= cache_size) return;          /* already available */

    /* Grow the array to 'required' elements */
    idx_t new_size = required;
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
    if (!new_cache) {
        exit(EXIT_FAILURE);   /* cannot proceed without memory */
    }
    cache = new_cache;

    /* If this is the first time, copy the initial 6 terms directly */
    if (cache_size == 0) {
        size_t m = new_size < 6 ? new_size : 6;
        for (size_t i = 0; i < m; ++i) cache[i] = init[i];
    }
    /* If we currently have fewer than 6 terms, fill the rest up to 6 */
    else if (cache_size < 6) {
        size_t start = cache_size;
        size_t end   = new_size < 6 ? new_size : 6;
        for (size_t i = start; i < end; ++i) cache[i] = init[i];
    }

    /* Compute any remaining terms beyond the first 6 */
    size_t start_idx = cache_size < 6 ? 6 : cache_size;
    for (size_t i = start_idx; i < new_size; ++i) {
        /* recurrence uses indices relative to cache (0-based) */
        seq_t val = c448          * cache[i-1];
        val -= c46080          * cache[i-2];
        val += c491520         * cache[i-3];
        val += c37748736       * cache[i-4];
        val -= c637534208      * cache[i-5];
        val += c2147483648     * cache[i-6];
        cache[i] = val;
    }

    cache_size = new_size;
}

seq_t
a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    ensure_terms(n);
    return cache[n-1];                   /* 0‑based to 1‑based */
}
