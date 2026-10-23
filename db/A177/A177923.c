/*
 * A177923  a(1)=19, a(2)=13, a(3)=37; thereafter a(n) =
 *          gpf(a(n-1)+a(n-2)+a(n-3)), where gpf = "greatest prime factor".
 * https://oeis.org/A177923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=19, a(2)=13, a(3)=37; for n>3
 *            a(n)=gpf(a(n-1)+a(n-2)+a(n-3)) (gpf = greatest prime factor)
 * Range:     u8 n <= 28, u16 n <= 212, u32 n <= 212, u64 n <= 212,
 *            u128 n <= 212, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;

    /* Ensure the cache can hold indices up to n */
    if (cache_cap <= (size_t)n) {
        size_t old_cap = cache_cap;
        size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure → zero */
        cache = tmp;
        cache_cap = new_cap;
        /* No need to touch new cells; they will be filled on demand */
    }

    /* Initialise the first three terms if not already present */
    if (cache[1] == (seq_t)0) {
        cache[1] = (seq_t)19;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)37;
    }

    if (last_computed < 3) last_computed = 3;

    /* Compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        uint64_t sum64 =
            (uint64_t)cache[i-1] + (uint64_t)cache[i-2] + (uint64_t)cache[i-3];
        uint64_t gpf64 = gpf(sum64);
        cache[i] = (seq_t)gpf64;
    }
    last_computed = n;

    return cache[n];
}
