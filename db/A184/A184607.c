/*
 * A184607  Half the number of (n+1) X 3 binary arrays with no 2 X 2 subblock
 *          sum differing from a horizontal or vertical neighbor subblock sum
 *          by more than one.
 * https://oeis.org/A184607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence definition for OEIS A184607
   a(n) = 5*a(n-1) + 3*a(n-2) + 10*a(n-3) + 29*a(n-4)
          - 66*a(n-5) - 20*a(n-6) + 8*a(n-7) - 12*a(n-8) + 8*a(n-9)
   with initial values
   a(1)=28, a(2)=160, a(3)=918, a(4)=5430, a(5)=32042,
   a(6)=188394, a(7)=1107926, a(8)=6519094, a(9)=38356642 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Invariant: cache[1..computed_max] contain a(1..computed_max) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size of cache (in terms)    */
    static idx_t computed_max = 0; /* highest index that has been computed */

    if (n < 1) return (seq_t)0;   /* out of range */

    /* Ensure the cache can hold index n */
    if (cache_size <= n) {
        size_t new_size = (size_t)n + 1;               /* +1 so index n is valid          */
        /* realloc returns void* in C, void* in C++ as well; cast to seq_t* */
        void *tmp = realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = (seq_t*)tmp;

        /* initialise newly allocated slots to zero                */
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;

        cache_size = new_size;
    }

    /* Initial terms (immutable, required by the recurrence) */
    static const seq_t init[9] = {
        (seq_t)28ULL,     /* a(1) */
        (seq_t)160ULL,    /* a(2) */
        (seq_t)918ULL,    /* a(3) */
        (seq_t)5430ULL,   /* a(4) */
        (seq_t)32042ULL,  /* a(5) */
        (seq_t)188394ULL, /* a(6) */
        (seq_t)1107926ULL,/* a(7) */
        (seq_t)6519094ULL,/* a(8) */
        (seq_t)38356642ULL/* a(9) */
    };

    /* Compute any missing values up to n (memoising) */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init[i - 1];
        } else {
            seq_t pos = 5 * cache[i - 1];
            pos += 3 * cache[i - 2];
            pos += 10 * cache[i - 3];
            pos += 29 * cache[i - 4];
            pos += 8 * cache[i - 7];
            pos += 8 * cache[i - 9];

            seq_t neg = 66 * cache[i - 5];
            neg += 20 * cache[i - 6];
            neg += 12 * cache[i - 8];

            cache[i] = pos - neg;
        }
    }

    computed_max = n;
    return cache[n];
}
