/*
 * A069983  Number of 5-gonal compositions of n into positive parts.
 * https://oeis.org/A069983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3) - 6*a(n-4) + 6*a(n-5) +
 *            4*a(n-6) - 4*a(n-7) - a(n-8) + a(n-9) Initial values:
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,
 *            a(5)=1,a(6)=5,a(7)=15,a(8)=35,a(9)=65
 * Range:     u8 n <= 11, u16 n <= 40, u32 n <= 623, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation storage */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = (idx_t)(-1);   /* indicates uninitialised */

    /* initial values for n = 0..9 */
    static const seq_t init[10] = {0,0,0,0,0,1,5,15,35,65};

    /* initialisation on first call */
    if (cache == NULL) {
        cache_size = 10;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = init[i];
        max_computed = 9;
    }

    /* return early if the desired value is already known */
    if (n < cache_size && !((n > max_computed) && (n < 10))) {
        if (n <= max_computed)
            return cache[n];
        /* n < 10 but higher than max_computed: fill in the missing values */
    }

    /* strictly ensure cache large enough */
    if (n >= cache_size) {
        size_t new_size = cache_size;
        if (new_size == 0) new_size = 10;
        while (new_size <= n)
            new_size <<= 1;  /* double until sufficient */

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* compute from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Build the recurrence safely: add all positive terms first,
           then subtract negatives to avoid underflow on unsigned types. */
        seq_t value = cache[i-1];
        value += 4 * cache[i-2];
        value += 6 * cache[i-5];
        value += 4 * cache[i-6];
        value += cache[i-9];
        value -= 4 * cache[i-3];
        value -= 6 * cache[i-4];
        value -= 4 * cache[i-7];
        value -= cache[i-8];
        cache[i] = value;
    }

    max_computed = n;
    return cache[n];
}
