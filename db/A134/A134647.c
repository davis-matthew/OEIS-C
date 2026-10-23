/*
 * A134647  Let a(1) = 0, a(2) = 1; and a(n) = a(n-1) + [the two-digit integer
 *          split by the comma which separates a(n-1) and a(n-2)].
 * https://oeis.org/A134647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=0; a(2)=1; a(n)=a(n-1)+ (last_digit(a(n-2))*10 +
 *            first_digit(a(n-1))) for n>=3
 * Range:     u8 n <= 8, u16 n <= 1535, u32 n <= 9247, u64 n <= 9247,
 *            u128 n <= 9247, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored (>=2 once initialised) */
    static size_t cache_cap  = 0;   /* capacity of the cache array */

    /* Helper to increase cache capacity */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        cache[0] = 0;   /* a(1) */
        cache[1] = 1;   /* a(2) */
        cache_size = 2;
    }

    /* Ensure the cache can hold at least n terms (n is 1‑based) */
    while (cache_size < (size_t)n) {
        /* grow capacity if needed */
        if (cache_size + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Indices for the recurrence (zero based) */
        seq_t prev2 = cache[cache_size - 2];   /* a(n-2) */
        seq_t prev1 = cache[cache_size - 1];   /* a(n-1) */

        /* last digit of a(n-2) */
        seq_t last = prev2 % ((seq_t)10);

        /* first digit of a(n-1) */
        seq_t first = prev1;
        while (first >= ((seq_t)10)) first /= (seq_t)10;

        /* two‑digit number formed by [last][first] */
        seq_t form = last * ((seq_t)10) + first;

        /* a(n) */
        cache[cache_size] = prev1 + form;
        cache_size++;
    }

    /* return the desired term (n is 1‑based) */
    return cache[n - 1];
}
