/*
 * A029030  Expansion of 1/((1-x)(1-x^2)(1-x^10)(1-x^11)).
 * https://oeis.org/A029030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-10) - 2*a(n-12) + a(n-14)
 *            - a(n-21) + a(n-22) + a(n-23) - a(n-24) with a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=2, a(4)=3, a(5)=3, a(6)=4, a(7)=4, a(8)=5, a(9)=5,
 *            a(10)=7, a(11)=8, a(12)=10, a(13)=11, a(14)=13, a(15)=14,
 *            a(16)=16, a(17)=17, a(18)=19, a(19)=20, a(20)=23,...
 * Range:     u8 n <= 57, u16 n <= 430, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static const seq_t base_vals[24] = {
    1, 1, 2, 2, 3, 3, 4, 4, 5, 5,
    7, 8,10,11,13,14,16,17,19,20,
   23,25,29,31
};

static seq_t *cache = NULL;   /* dynamic array holding computed terms   */
static size_t cache_cap = 0;  /* allocated size of the array             */
static size_t last_computed = 0; /* largest index that has been computed   */
static int base_initialized = 0; /* flag, 0 = not yet, 1 = yes          */

seq_t a(idx_t n)
{
    /* Convert to size_t for internal indexing (n is uint64_t). */
    size_t idx = (size_t)n;

    /* Return stored value if it is one of the first 24 terms. */
    if (idx < 24)
        return base_vals[idx];

    /* Ensure the cache can hold all indices up to idx. */
    if (idx + 1 > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 24;
        while (new_cap <= idx) new_cap <<= 1; /* double until large enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Load the first 24 coefficients once. */
    if (!base_initialized) {
        for (size_t i = 0; i < 24; ++i)
            cache[i] = base_vals[i];
        base_initialized = 1;
        last_computed = 23;   /* indices 0..23 are now available       */
    }

    /* If the requested index is already available, answer immediately. */
    if (idx <= last_computed)
        return cache[idx];

    /* Compute values needed from last_computed+1 up to idx using the
       recurrence, which is valid for n ≥ 24. */
    size_t start = last_computed + 1;
    for (size_t i = start; i <= idx; ++i) {
        seq_t val = cache[i-1] + cache[i-2] - cache[i-3];
        val += cache[i-10];
        val -= cache[i-12];
        val -= cache[i-12];          /* -2*a(i-12)                        */
        val += cache[i-14];
        val -= cache[i-21];
        val += cache[i-22];
        val += cache[i-23];
        val -= cache[i-24];
        cache[i] = val;
    }

    last_computed = idx;
    return cache[idx];
}
