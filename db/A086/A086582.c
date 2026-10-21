/*
 * A086582  First 2^n terms of the self-convolution generates the next 2^n
 *          terms of this sequence after the 2^n-th term, for all n>=0, where
 *          a(0)=1.
 * https://oeis.org/A086582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1.
   For every k ≥ 0 let m = 2^k.  
   When the first 2^k terms a[0]..a[m‑1] are known,
   the next 2^k terms a[m]..a[2m‑1] are given by the
   self‑convolution of the first block:
        a[m + i] = Σ_{j=0}^{i} a[j] * a[i – j]   for 0 ≤ i < m. */

#include <stdlib.h>   /* malloc, realloc, abort */

/* Static memoisation cache.  The build system supplies the
   integer types:  seq_t  (value type) and  idx_t  (unsigned index). */

static seq_t *cache      = NULL;   /* array of computed terms          */
static idx_t cache_len   = 0;      /* number of terms already computed */
static idx_t cache_cap   = 0;      /* current capacity of 'cache'      */

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_cap(idx_t needed)
{
    if (cache_cap >= needed)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    /* C++ requires a cast from void* to the desired pointer type. */
    seq_t *new_buf = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!new_buf) {
        /* Allocation failed – terminate the program. */
        abort();
    }
    cache = new_buf;
    cache_cap = new_cap;
}

/* External linkage as required by the grading harness. */
seq_t a(idx_t n)
{
    /* Initialize the sequence with a(0)=1. */
    if (cache_len == 0) {
        ensure_cap(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute additional blocks until index n is available. */
    while (cache_len <= n) {
        idx_t block = cache_len;          /* current block size (2^k)   */
        idx_t next_len = block * 2;       /* length after adding block */

        ensure_cap(next_len);

        /* Self‑convolution of the first 'block' terms. */
        for (idx_t i = 0; i < block; ++i) {
            seq_t sum = 0;
            for (idx_t j = 0; j <= i; ++j) {
                sum += cache[j] * cache[i - j];
            }
            cache[block + i] = sum;
        }

        cache_len = next_len;            /* new total length          */
    }

    return cache[n];
}
