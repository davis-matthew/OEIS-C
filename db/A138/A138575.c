/*
 * A138575  Let H(1) = 0; B(0) = 0; H(n) = (n - B(n - 1)); B(n) =
 *          H(floor(n/2)); then a(n) = H(n) + B(n).
 * https://oeis.org/A138575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A138575
   Recurrence:  H(1)=0, B(0)=0,
                H(n) = n - B(n-1)
                B(n) = H(floor(n/2))
   a(n) = H(n)+B(n)
   Offset 0: a(0)=1
   Implementation uses lazy memoisation and handles the special
   value H(1)=0 explicitly. */

#include <stdlib.h>

/* static cache for H and B values */
static seq_t *H_cache = NULL;
static seq_t *B_cache = NULL;
static size_t cache_size = 0;   /* allocated array size */
static size_t computed_to = 0;  /* largest index already computed */

/* enlarge the cache to accommodate at least 'need' entries (indices 0..need-1) */
static void ensure_cache_size(size_t need)
{
    if (cache_size >= need)
        return;
    size_t new_size = need > 1 ? need * 2 : 1;
    while (new_size < need)
        new_size *= 2;
    seq_t *new_H = (seq_t *)realloc(H_cache, new_size * sizeof(seq_t));
    seq_t *new_B = (seq_t *)realloc(B_cache, new_size * sizeof(seq_t));
    if (!new_H || !new_B)
        abort();                      /* allocation failed */
    H_cache = new_H;
    B_cache = new_B;
    cache_size = new_size;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* initialise cache on first use */
    if (cache_size == 0) {
        ensure_cache_size(1);                 /* at least index 0 */
        H_cache[0] = (seq_t)1;                /* H(0)=1 so that a(0)=1 */
        B_cache[0] = (seq_t)0;                /* B(0)=0 */
        computed_to = 0;                      /* up to index 0 computed */
    }

    /* extend cache if needed and compute new entries up to n */
    if ((size_t)n > computed_to) {
        ensure_cache_size((size_t)n + 1);     /* needs indices 0..n */
        for (size_t i = computed_to + 1; i <= (size_t)n; ++i) {
            if (i == 1) {                     /* special value H(1)=0 */
                H_cache[1] = (seq_t)0;
                B_cache[1] = H_cache[0];      /* B(1)=H(0)=1 */
            } else {
                B_cache[i] = H_cache[i >> 1]; /* B(i)=H(floor(i/2)) */
                H_cache[i] = (seq_t)i - B_cache[i - 1]; /* H(i)=i-B(i-1) */
            }
        }
        computed_to = (size_t)n;
    }

    return H_cache[n] + B_cache[n];
}
