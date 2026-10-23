/*
 * A175138  a(n) = Product_{i=1..n-1} a(i), with a(1) = 3 and a(2) = 5.
 * https://oeis.org/A175138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..n-1} a(i), a(1)=3, a(2)=5
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
static size_t cap = 0;        /* current allocated capacity for `cache` */
static idx_t max_n = 0;       /* largest n for which cache is populated */

/* Return a(n) for any n >= 1.  The function memoizes results
 * and works regardless of the order of calls. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1; return 0 for safety */

    /* Make the cache large enough to hold n terms */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while ((size_t)n > new_cap) new_cap *= 2;
#if defined(__cplusplus)
        seq_t *tmp = static_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) {
            /* Allocation failed – abort program (no I/O as required) */
            abort();
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Ensure the first two terms are stored */
    if (max_n < 1) {
        cache[0] = (seq_t)3;
        max_n = 1;
    }
    if (max_n < 2) {
        cache[1] = (seq_t)5;
        max_n = 2;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 3) {
            /* a(3) = a(1) * a(2) */
            cache[i - 1] = cache[0] * cache[1];
        } else {
            /* For i >= 4, a(i) = a(i-1)^2 as given by the recurrence */
            cache[i - 1] = cache[i - 2] * cache[i - 2];
        }
    }
    max_n = n;

    return cache[n - 1];
}
