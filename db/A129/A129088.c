/*
 * A129088  a(1)=a(2)=1; a(n) = gcd(a(n-1), n) + gcd(a(n-2), n).
 * https://oeis.org/A129088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1)=1, a(2)=1; * a(n)=gcd(a(n-1),n)+gcd(a(n-2),n) for n>=3
 * Range:     u8 n <= 592, u16 n <= 9717, u32 n <= 9717, u64 n <= 9717,
 *            u128 n <= 9717, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"

#ifdef __cplusplus
    /* C++: use new/delete for array allocation of arbitrary‑precision types */
    static seq_t* allocate_seq_array(size_t n) { return new seq_t[n]; }
    static void deallocate_seq_array(seq_t* p) { delete[] p; }
#else
    /* C: use malloc/free for ordinary integer types */
    #include <stdlib.h>  /* malloc, free */
    static seq_t* allocate_seq_array(size_t n) { return (seq_t*)malloc(n * sizeof(seq_t)); }
    static void deallocate_seq_array(seq_t* p) { free(p); }
#endif

seq_t a(idx_t n) {
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_max  = 0;

    /* --- initialization (only once) --- */
    if (!cache) {
        cache_cap = 3;                /* space for indices 0,1,2 */
        cache = allocate_seq_array(cache_cap);
        if (!cache) abort();

        /* base values (a(1)=a(2)=1) */
        cache[0] = 0;  /* unused but keeps the array 1‑based */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_max = 2;
    }

    /* --- grow array if needed --- */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = allocate_seq_array(new_cap);
        if (!tmp) abort();

        /* copy old values */
        for (size_t i = 0; i <= cache_max; ++i) tmp[i] = cache[i];
        deallocate_seq_array(cache);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* --- compute missing terms up to n --- */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t g1 = gcd(cache[i-1], (seq_t)i);
        seq_t g2 = gcd(cache[i-2], (seq_t)i);
        cache[i] = g1 + g2;
    }
    if (n > cache_max) cache_max = n;

    return cache[n];
}
