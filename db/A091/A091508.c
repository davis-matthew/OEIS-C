/*
 * A091508  Least k such that b(k) = 1, where b(1) = n and b(k+1) = b(k)+k+1
 *          if gcd(k+1, b(k)) = 1 or else b(k)/gcd(k+1, b(k)).
 * https://oeis.org/A091508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  a(n) is the least k such that b(k)=1, where
 * b(1)=n and for k≥1
 *   if gcd(k+1,b(k))==1  then b(k+1)=b(k)+(k+1)
 *   else                    b(k+1)=b(k)/gcd(k+1,b(k))
 * The function evaluates this iteratively and caches all values
 * computed up to the maximum argument supplied.  The caching
 * mechanism ensures each a(n) is computed only once, giving
 * O(total steps) overall for repeated calls. */

#include "../utils/gcd.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* cached values for a(1), a(2), …  (cache_len == number of
 * initialized entries */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* ------------------------------------------------------------------ */
/* Compute a(n) for n ≥ 1, returning result of type seq_t.           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, but return 0 */

    /* Remember how many entries were cached before possibly enlarging. */
    size_t old_len = cache_len;

    /* Enlarge storage if necessary (realloc/malloc). */
    if (n > cache_len) {
        size_t new_len = (size_t)n;                      /* new size in bytes */
        seq_t *tmp = NULL;
        if (cache_len == 0) {
            tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        }
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute all missing a(i) values from old_len+1 up to n. */
    for (idx_t i = old_len + 1; i <= n; ++i) {
        seq_t b = (seq_t)i;       /* b(1) = i */
        idx_t k = 1;              /* current index in the b sequence */

        /* Iterate until b becomes 1, counting the steps. */
        while (b != (seq_t)1) {
            seq_t step = (seq_t)(k + 1);
            seq_t g = gcd(step, b);
            if (g == (seq_t)1) {
                b = b + step;
            } else {
                b = b / g;
            }
            ++k;
        }

        /* Store the computed a(i).  a(i) == k (the last k reached). */
        cache[i - 1] = (seq_t)k;
    }

    return cache[n - 1];
}
