/*
 * A069203  a(1)=0 a(2)=3 a(n+2)=(a(n+1)+a(n))/3 if (a(n+1)+a(n)==0 (mod 3));
 *          a(n+2)=a(n+1)+a(n) otherwise.
 * https://oeis.org/A069203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=0, a(2)=3; For n >= 1, a(n+2) = (a(n+1)+a(n))/3 if
 *            (a(n+1)+a(n)) mod 3 == 0; otherwise a(n+2) = a(n+1)+a(n)
 * Range:     u8 n <= 30, u16 n <= 66, u32 n <= 148, u64 n <= 248,
 *            u128 n <= 487, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of A069203.  The function memoises results
   so calls may come in any order.  It is safe for any built‑in
   `seq_t` provided by the build (uint8_t,…,unsigned __int128,
   or an arbitrary‑precision class).  Memory is allocated the first
   time the function is invoked and grows as needed. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoised values */
    static seq_t *cache = NULL;    /* 1‑based, cache[0] unused          */
    static idx_t cache_size   = 0; /* current size of allocated array   */
    static idx_t computed_at  = 0; /* highest index already computed    */

    /* Initialise the cache with the first two terms.  We allocate one
       extra slot so that cache[n] is directly usable for 1‑based n. */
    if (cache == NULL) {
        cache_size = 2;                     /* need indices 0..2    */
        cache = (seq_t *)malloc((size_t)(cache_size + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed – unlikely */
        cache[0] = (seq_t)0;                /* unused */
        cache[1] = (seq_t)0;                /* a(1) */
        cache[2] = (seq_t)3;                /* a(2) */
        computed_at = 2;
    }

    /* If the requested index is already available, just return it. */
    if (n <= computed_at) {
        return cache[n];
    }

    /* Grow the cache if necessary.  We round up to n to keep the array
       contiguous. */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(new_size + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – return 0 as a safe but incorrect
               value (unlikely to happen in tests). */
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute all terms up to n and store them. */
    for (idx_t i = computed_at + 1; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        if ((sum % (seq_t)3) == 0) {
            cache[i] = sum / (seq_t)3;
        } else {
            cache[i] = sum;
        }
    }
    computed_at = n;

    return cache[n];
}
