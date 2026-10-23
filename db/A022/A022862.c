/*
 * A022862  a(n) = [ a(n-1)/a(1) ] + [ a(n-1)/a(2) ] + ... + [ a(n-1)/a(n-1) ]
 *          for n >= 3, with initial terms 1,2.
 * https://oeis.org/A022862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n>=3 a(n)=⌊a(n-1)/a(1)⌋+...+⌊a(n-1)/a(n-1)⌋
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *cache = NULL;   /* memoized values, indices start at 1 */
static bool *done = NULL;     /* flags for computed entries */
static idx_t cache_cap = 0;   /* current allocated capacity */

/* Ensure the cache can hold indices up to n.  Allocates or resizes arrays.  */
static void ensure_capacity(idx_t n) {
    if (n + 1 < cache_cap) return;          /* already large enough */
    idx_t new_cap = n + 1;
    if (new_cap < 16) new_cap = 16;          /* avoid tiny allocations */

    seq_t *new_cache;
    bool *new_done;

#if defined(__cplusplus)
    new_cache = new seq_t[new_cap];
    new_done  = new bool[new_cap];
    if (cache) {                            /* copy existing contents */
        for (idx_t i = 1; i < cache_cap; ++i) new_cache[i] = cache[i];
        delete [] cache;
        delete [] done;
    }
#else
    new_cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
    new_done  = (bool*)calloc(new_cap, sizeof(bool));
    if (cache) {                            /* copy existing contents */
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
        free(done);
    }
#endif
    cache = new_cache;
    done  = new_done;
    cache_cap = new_cap;
}

/* Return the n-th term of OEIS sequence A022862.  */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for 0, but safe */

    ensure_capacity(n);

    /* initialise the base terms if they are not yet computed */
    if (!done[1]) { cache[1] = (seq_t)1; done[1] = true; }
    if (!done[2]) { cache[2] = (seq_t)2; done[2] = true; }

    if (!done[n]) {
        for (idx_t i = 3; i <= n; ++i) {
            if (done[i]) continue;
            seq_t prev = cache[i-1];
            seq_t sum  = 0;
            for (idx_t k = 1; k < i; ++k) {
                sum += prev / cache[k];
            }
            cache[i] = sum;
            done[i]  = true;
        }
    }
    return cache[n];
}
