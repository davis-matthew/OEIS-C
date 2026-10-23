/*
 * A127681  a(0) = 1. a(n+1) = sum{k=0 to n} a(n-k)*a(ceiling(k/2)).
 * https://oeis.org/A127681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(n+1) = Σ_{k=0}^n a(n-k) * a(⌈k/2⌉)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised for arbitrary‑precision or fixed‑width
   integer types.  No external helper functions are used. */
#include <stdlib.h>

/* static state for the memoised cache */
static seq_t *cache = NULL;          /* pointer to stored a(n) values      */
static idx_t  capacity      = 0;     /* number of elements currently
                                       allocated in cache                */
static idx_t  computed_upto = (idx_t)-1;  /* highest n previously computed */

/* Ensure that the cache can store values up to max_n (inclusive). */
static void ensure_cache(idx_t max_n)
{
    if (capacity <= max_n) {
        idx_t old_cap = capacity;
        idx_t new_cap = max_n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);            /* allocation failed            */
        cache   = tmp;
        capacity = new_cap;
        /* initialise newly allocated entries to zero */
        for (idx_t i = old_cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
    }
}

/* Compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    /* One‑time initialisation for a(0) */
    if (computed_upto == (idx_t)-1) {
        ensure_cache(0);
        cache[0] = (seq_t)1;
        computed_upto = 0;
    }

    if (n <= computed_upto)          /* already known */
        return cache[n];

    /* Compute all values up to n inclusively */
    ensure_cache(n);
    for (idx_t cur = computed_upto + 1; cur <= n; ++cur) {
        seq_t sum = (seq_t)0;
        /* cur corresponds to a(cur).  In the recurrence,
           a(cur) = Σ_{k=0}^{cur-1} a(cur-1-k) * a(⌈k/2⌉).       */
        for (idx_t k = 0; k < cur; ++k) {
            seq_t left  = cache[cur - 1 - k];
            seq_t right = cache[(k + 1) / 2];   /* ceil(k/2) = (k+1)/2   */
            sum += left * right;
        }
        cache[cur] = sum;
    }
    computed_upto = n;
    return cache[n];
}
