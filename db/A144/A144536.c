/*
 * A144536  Denominators of continued fraction convergents to sqrt(3)/2.
 * https://oeis.org/A144536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A144536: Denominators of continued fraction convergents to sqrt(3)/2
 * Recurrence: a(0)=1, a(1)=1, a(2)=7, a(3)=15,
 *              a(n) = 14*a(n-2) - a(n-4)  for n >= 4
 */

#include <stdlib.h>

/* Macro to convert integer literals to the appropriate seq_t type in a
   portable way for both C and C++.  In C++ we use a constructor; in C
   we use an explicit cast. */
#if defined(__cplusplus)
# define SEQ_VAL(x) seq_t(x)
#else
# define SEQ_VAL(x) ((seq_t)(x))
#endif

/* Cached sequence values.  The array is sized to accommodate indices
   up to the highest value computed so far. */
static seq_t *cache = NULL;
static idx_t   last_computed = (idx_t)-1;    /* No values computed yet */
static size_t  cache_cap     = 0;

/* Ensure that cache contains values up to index `need`, extending it
   and filling any missing entries using the recurrence. */
static void ensure_cache(idx_t need)
{
    /* First call: initialize with the base cases. */
    if (cache == NULL || cache_cap == 0) {
        size_t initcap = 4;
        if (initcap <= (size_t)need + 1) initcap = (size_t)need + 1;
        cache = (seq_t *)malloc(initcap * sizeof(seq_t));
        if (cache == NULL) exit(1);            /* allocation failure aborts */
        cache[0] = SEQ_VAL(1);
        cache[1] = SEQ_VAL(1);
        cache[2] = SEQ_VAL(7);
        cache[3] = SEQ_VAL(15);
        cache_cap = initcap;
        last_computed = 3;
    }

    /* Enlarge if necessary. */
    if ((size_t)need + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)need + 1) new_cap = (size_t)need + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms. */
    for (idx_t i = last_computed + 1; i <= need; ++i) {
        cache[i] = SEQ_VAL(14) * cache[i - 2] - cache[i - 4];
    }

    if (need > last_computed) last_computed = need;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
