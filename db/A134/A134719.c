/*
 * A134719  Odd Padovan numbers.
 * https://oeis.org/A134719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-12) + a(n-8) + 7*a(n-4); Initial values (1‑based):
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=1, a(6)=3, a(7)=5, a(8)=7,
 *            a(9)=9, a(10)=21, a(11)=37, a(12)=49
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache      = NULL; /* 1‑based cache (cache[0] unused) */
static size_t cache_cap  = 0;     /* allocated capacity (including index 0) */
static idx_t  cache_size = 0;     /* highest n that has been computed */

/* Ensure the cache contains terms up to n (inclusive).  The cache
   is grown as necessary. */
static void
ensure_up_to(idx_t n)
{
    if (n == 0) return;

    /*--- Allocate or grow the cache ------------------------------------*/
    size_t needed = (size_t)n + 1;        /* allow index 0 to n */
    if (cache == NULL) {
        if (needed < 16) needed = 16;
        cache     = seq_alloc_new_array(needed);
        cache_cap = needed;
    } else if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (needed > new_cap) new_cap <<= 1;
        seq_t *tmp = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cache_cap; ++i) tmp[i] = cache[i];
        seq_free_array(cache);
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /*--- Initialize base terms when the cache is empty -----------------*/
    if (cache_size == 0) {
        /* bounds: we always fill all 12 initial terms for future
           recurrence, even if n < 12. */
        if (n < 12) n = 12;
        cache[1]  = (seq_t)1;
        cache[2]  = (seq_t)1;
        cache[3]  = (seq_t)1;
        cache[4]  = (seq_t)1;
        cache[5]  = (seq_t)1;
        cache[6]  = (seq_t)3;
        cache[7]  = (seq_t)5;
        cache[8]  = (seq_t)7;
        cache[9]  = (seq_t)9;
        cache[10] = (seq_t)21;
        cache[11] = (seq_t)37;
        cache[12] = (seq_t)49;
        cache_size = 12;
    }

    /*--- Compute missing terms by the recurrence -----------------------*/
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t term = cache[i - 12];
        term += cache[i - 8];
        term += cache[i - 4] * (seq_t)7;
        cache[i] = term;
    }
    if (n > cache_size) cache_size = n;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_up_to(n);
    return cache[n];
}
