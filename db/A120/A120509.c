/*
 * A120509  Generalized meta-Fibonacci sequence a(n) with parameters s=2 and
 *          k=4.
 * https://oeis.org/A120509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2-a(n-1)) + a(n-3-a(n-2)) + a(n-4-a(n-3)) +
 *            a(n-5-a(n-4)) for n > 6, with initial values a(1)=a(2)=a(3)=1,
 *            a(4)=2, a(5)=3, a(6)=4
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow arbitrary access order */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated capacity (number of elements) */
    static size_t last = 0;     /* highest index computed so far */

    if (n == 0) return 0;

    /* Ensure sufficient capacity */
    if (cache == NULL) {
        /* Initial allocation: at least up to the requested n */
        cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failed; fallback */
        cache[0] = 0;                  /* unused but keep safe */
    } else if (cap < (size_t)n + 1) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute new values up to n */
    if (last < (size_t)n) {
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            if (i <= 3) {
                cache[i] = 1;
            } else if (i <= 6) {
                cache[i] = (seq_t)i - 2;
            } else {
                /* Compute indices for the recurrence */
                size_t idx1 = (size_t)i - 2 - (size_t)cache[i - 1];
                size_t idx2 = (size_t)i - 3 - (size_t)cache[i - 2];
                size_t idx3 = (size_t)i - 4 - (size_t)cache[i - 3];
                size_t idx4 = (size_t)i - 5 - (size_t)cache[i - 4];
                cache[i] = cache[idx1] + cache[idx2] + cache[idx3] + cache[idx4];
            }
        }
        last = (size_t)n;
    }

    return cache[(size_t)n];
}
