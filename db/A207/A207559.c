/*
 * A207559  Number of n X 3 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 6*a(n-2) - 21*a(n-3) + 36*a(n-4) * + 12*a(n-5)
 *            - 39*a(n-6) + 6*a(n-7) + 10*a(n-8) - 3*a(n-9), * for n > 10. *
 *            Initial values: a(1) = 6, a(2) = 36, a(3) = 92, a(4) = 221, *
 *            a(5) = 618, a(6) = 1690, a(7) = 4861, a(8) = 13900, * a(9) =
 *            40452, a(10) = 117717. * * The function uses m...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init[10] = {
    (seq_t)6,     (seq_t)36,     (seq_t)92,     (seq_t)221,
    (seq_t)618,   (seq_t)1690,   (seq_t)4861,   (seq_t)13900,
    (seq_t)40452, (seq_t)117717
};

static seq_t *cache = NULL;      /* array of computed terms a(1), a(2), ... */
static size_t cache_cap = 0;     /* allocated capacity of cache      */
static idx_t cache_len = 0;      /* number of terms actually stored  */

static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    if ((size_t)n > cache_cap) {           /* grow the buffer if required */
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* out of memory: terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill in the missing values */
    for (idx_t i = cache_len; i < n; ++i) {
        if (i < 10) {                       /* use the given initial terms */
            cache[i] = init[i];
        } else {
            idx_t j = i;                    /* j = i = index of a(j+1) */
            /* use the recurrence written as (positive sum) – (negative sum) */
            seq_t pos = 6 * cache[j-1];
            pos += 36 * cache[j-4];
            pos += 12 * cache[j-5];
            pos += 6  * cache[j-7];
            pos += 10 * cache[j-8];

            seq_t neg = 6 * cache[j-2];
            neg += 21 * cache[j-3];
            neg += 39 * cache[j-6];
            neg += 3  * cache[j-9];

            cache[i] = pos - neg;            /* secure subtraction; pos ≥ neg */
        }
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    /* a(n) is defined for n ≥ 1.  For n < 1 we return 0. */
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[n-1];
}
