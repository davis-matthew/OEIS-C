/*
 * A183684  Number of (n+1) X 6 binary arrays with every 2 X 2 subblock
 *          nonsingular.
 * https://oeis.org/A183684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 18*a(n-2) - 20*a(n-3) - 72*a(n-4) for n >= 5
 *            (initial values: a(1) = 96, a(2) = 528, a(3) = 2688, a(4) =
 *            14144)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* External linkage required */
seq_t a(idx_t n)
{
    /* Empty or negative n is not expected, but guard anyway */
    if (n == 0) return 0;

    /* static cache of computed values: cache[0] = a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of terms currently cached */

    /* Expand cache if needed */
    if (cache_len < n) {
        /* Determine new size */
        idx_t new_len = n;
        /* Reallocate if cache exists, otherwise allocate fresh */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (cache == NULL) exit(EXIT_FAILURE); /* allocation failed */
            cache_len = 0;
        } else if (cache_len < new_len) {
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (tmp == NULL) exit(EXIT_FAILURE);
            cache = tmp;
        }

        /* Fill missing entries */
        while (cache_len < new_len) {
            seq_t val;
            idx_t idx = cache_len + 1;  /* index of term to compute (1‑based) */
            if (idx <= 4) {
                /* initial values */
                switch (idx) {
                    case 1: val = 96;     break;
                    case 2: val = 528;    break;
                    case 3: val = 2688;   break;
                    case 4: val = 14144;  break;
                    default: val = 0;     break; /* should never happen */
                }
            } else {
                /* recurrence: a(n)=3*a(n-1)+18*a(n-2)-20*a(n-3)-72*a(n-4) */
                seq_t t1 = 3 * cache[cache_len - 1];           /* a(n-1) */
                seq_t t2 = 18 * cache[cache_len - 2];          /* a(n-2) */
                seq_t sub = 20 * cache[cache_len - 3] + 72 * cache[cache_len - 4];
                val = t1 + t2;
                val -= sub;   /* subtraction is safe for this recurrence */
            }
            cache[cache_len] = val;
            cache_len++;
        }
    }

    return cache[n - 1];
}
