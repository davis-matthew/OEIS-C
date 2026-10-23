/*
 * A143605  Polynomial expansion sequence : p(x)=1 + x - x^5 + x^9 + x^10.
 * https://oeis.org/A143605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + a(n-5) - a(n-9) - a(n-10), with a(k)=0 for k<0.
 *            The function keeps a static cache so that any order of calls is
 *            efficient. It works for any integer-type seq_t (fixed-size or
 *            arbitrary‑precision)
 * Range:     i8 n <= 41, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;   /* cache[i] = a(i) for 0 <= i < cache_len */
static size_t cache_len = 0;  /* number of valid elements in cache */

/* Allocate an array of seq_t of given length.
   In C++ we use new[] so constructors are called; in C we use malloc. */
static seq_t *alloc_seq_len(size_t len) {
#ifdef __cplusplus
    return new seq_t[len];
#else
    return (seq_t *)malloc(len * sizeof(seq_t));
#endif
}

/* Reallocate the cache to the new length.
   Existing values are preserved.  In C++ we delete the old array
   after copying; in C we use realloc. */
static void grow_cache(size_t new_len) {
    if (new_len <= cache_len) return;          /* nothing to do */
#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_len];
    for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
    delete[] cache;
    cache = tmp;
#else
    seq_t *tmp = realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                         /* allocation failure */
    cache = tmp;
#endif
    cache_len = new_len;
}

static void compute_terms(size_t up_to) {
    /* Compute a(i) for all i from cache_len to up_to inclusive. */
    for (size_t i = cache_len; i <= up_to; ++i) {
        seq_t val = -cache[i - 1];             /* -a(i-1)  */
        if (i >= 5)  val += cache[i - 5];      /* +a(i-5)  */
        if (i >= 9)  val -= cache[i - 9];      /* -a(i-9)  */
        if (i >= 10) val -= cache[i -10];      /* -a(i-10) */
        cache[i] = val;
    }
}

seq_t a(idx_t n) {
    /* Ensure cache is large enough for index n. */
    size_t target = (size_t)n;
    if (cache_len == 0) {
        /* Initial allocation up to n (or at least 1). */
        grow_cache(target + 1);
        /* Zero all entries (not strictly required in C++, but safe). */
        for (size_t i = 0; i < cache_len; ++i) cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;                 /* a(0) = 1 */
        compute_terms(target);
    } else if (target >= cache_len) {
        /* Extend cache and compute new terms. */
        size_t old_len = cache_len;
        grow_cache(target + 1);
        /* Compute terms from old_len to target. */
        for (size_t i = old_len; i <= target; ++i) {
            seq_t val = -cache[i - 1];
            if (i >= 5)  val += cache[i - 5];
            if (i >= 9)  val -= cache[i - 9];
            if (i >= 10) val -= cache[i -10];
            cache[i] = val;
        }
    }
    return cache[target];
}
