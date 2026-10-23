/*
 * A143287  Number of binary words of length n containing at least one subword
 *          10^{7}1 and no subwords 10^{i}1 with i<7.
 * https://oeis.org/A143287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-8) - a(n-10) - a(n-17) Initial
 *            values: a(0)=0, a(1)=0, …, a(16)=8, a(17)=10 Recurrence holds
 *            for all n ≥ 18
 * Range:     u8 n <= 29, u16 n <= 53, u32 n <= 104, u64 n <= 209,
 *            u128 n <= 422, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* Basic cache infrastructure                                         */
    static seq_t *cache  = NULL;   /* pointer to allocated seq_t array     */
    static size_t capacity = 0;    /* current allocated size               */
    static size_t computed = 0;    /* highest index already computed       */

    /* ------------------------------------------------------------------ */
    /* Initial values up to n = 17                                         */
    static const seq_t init[18] = {
        (seq_t)0,  /* 0 */
        (seq_t)0,  /* 1 */
        (seq_t)0,  /* 2 */
        (seq_t)0,  /* 3 */
        (seq_t)0,  /* 4 */
        (seq_t)0,  /* 5 */
        (seq_t)0,  /* 6 */
        (seq_t)0,  /* 7 */
        (seq_t)0,  /* 8 */
        (seq_t)1,  /* 9 */
        (seq_t)2,  /*10 */
        (seq_t)3,  /*11 */
        (seq_t)4,  /*12 */
        (seq_t)5,  /*13 */
        (seq_t)6,  /*14 */
        (seq_t)7,  /*15 */
        (seq_t)8,  /*16 */
        (seq_t)10  /*17 */
    };

    /* ------------------------------------------------------------------ */
    /* Allocate space on first call                                       */
    if (cache == NULL) {
        /* Enough room for the 18 base terms and at least n+1 elements.   */
        capacity = 18;
        if ((size_t)n + 1 > capacity)
            capacity = (size_t)n + 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL)        /* allocation failed – undefined result */
            return (seq_t)0;

        /* Copy the base values */
        for (size_t i = 0; i < 18 && i < capacity; ++i)
            cache[i] = init[i];
        /* Zero the rest */
        for (size_t i = 18; i < capacity; ++i)
            cache[i] = (seq_t)0;
        computed = 17;          /* last index that is reliably known */
    }
    /* ------------------------------------------------------------------ */
    /* Grow the cache if we need more space                               */
    else if ((size_t)n + 1 > capacity) {
        size_t old_cap = capacity;
        size_t new_cap = capacity;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                 /* double until big enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)                   /* allocation failed   */
            return (seq_t)0;
        cache = tmp;
        /* Initialise only the *new* portions of the array                 */
        for (size_t i = old_cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        capacity = new_cap;
    }

    /* ------------------------------------------------------------------ */
    /* Fast return if the value is already known                         */
    if ((size_t)n <= computed)
        return cache[n];

    /* ------------------------------------------------------------------ */
    /* Compute missing terms using the recurrence                        */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        if (i < 18) {                     /* safety, though i>=18 here */
            cache[i] = init[i];
        } else {
            /* a(i) = 2*a(i-1) - a(i-2) + a(i-8) - a(i-10) - a(i-17) */ 
            seq_t term = cache[i - 1];
            term += cache[i - 1];          /* 2 * a(i-1) */
            term += cache[i - 8];          /* + a(i-8)    */

            seq_t sub = cache[i - 2];
            sub += cache[i - 10];
            sub += cache[i - 17];

            cache[i] = term - sub;        /* unsigned arithmetic; guaranteed non‑negative */
        }
    }
    computed = n;
    return cache[n];
}
