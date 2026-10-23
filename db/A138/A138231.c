/*
 * A138231  A009545 alternated with its first differences.
 * https://oeis.org/A138231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)…a(7) provided by the OEIS entry; for n > 7: a(n) = -4 *
 *            a(n-8). The recurrence holds for all n ≥ 8. The implementation
 *            memoises terms so that calls to a(n) in any order return the
 *            correct value
 * Range:     i8 n <= 30, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised terms */
static size_t cache_size = 0;    /* number of entries currently stored */

/* Ensure that cache[0 .. n] are computed. */
static void ensure_cache(idx_t n)
{
    /* If we already have the required range, nothing to do */
    if (cache_size > (size_t)n)
        return;

    /* Allocate a larger array.  In C++ the result of realloc must be cast. */
    size_t new_sz = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (tmp == NULL)
        return;                  /* allocation failure – leave cache unchanged */

    seq_t *old_cache = cache;
    cache = tmp;
    size_t old_sz = cache_size;
    cache_size = new_sz;

    /* Fill missing entries.  We fill sequentially from old_sz up to n. */
    for (idx_t i = old_sz; i <= n; ++i) {
        if ((size_t)i < 8) {
            /* Hard‑coded initial values.  seq_t may be an integer type or a
               big‑integer class with an implicit conversion from int. */
            switch (i) {
                case 0: cache[i] = 0; break;
                case 1: cache[i] = 1; break;
                case 2: cache[i] = 1; break;
                case 3: cache[i] = 1; break;
                case 4: cache[i] = 2; break;
                case 5: cache[i] = 0; break;
                case 6: cache[i] = 2; break;
                case 7: cache[i] = -2; break;
            }
        } else {
            /* Apply the recurrence a(n) = -4 * a(n-8). */
            cache[i] = - ((seq_t)4 * cache[i - 8]);
        }
    }
}

/* Public entry point.  Returns a(n). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
