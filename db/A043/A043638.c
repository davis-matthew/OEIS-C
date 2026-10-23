/*
 * A043638  Numbers whose base-10 representation has exactly 2 runs.
 * https://oeis.org/A043638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 670, u32 n <= 3055, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A043638: numbers whose base‑10 representation has exactly two runs.
   The code generates all two‑run numbers up to the minimal digit length that yields at least n
   terms, sorts them, and returns the (n‑1)‑th value.
*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* compare two seq_t values; works for built‑in integer types and for
   arbitrary‑precision types that support operator< and operator>. */
static int seq_compare(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Cache of already generated two‑run numbers. */
    static seq_t *cache      = NULL;   /* sorted array of seq_t */
    static idx_t  cache_len  = 0;     /* number of elements in cache */

    if (!cache || n > cache_len) {
        /* Find the minimal maximal length L such that the total number of
           two‑run numbers with up to L digits is at least n.
           For each length ℓ ≥ 2 we have 81·(ℓ−1) numbers. */
        idx_t  L = 2;
        idx_t  cumulative = 0;
        while (cumulative < n) {
            cumulative += 81 * (L - 1);
            ++L;
        }
        /* L is one past the required maximum length.  L-1 is the number
           of digits needed. */
        size_t maxlen = (size_t)(L - 1);
        size_t total_terms = (size_t)cumulative;

        /* Allocate memory for the new cache (cast needed for C++). */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * total_terms);
        if (!new_cache) exit(EXIT_FAILURE);

        /* Fill the array with all two‑run numbers of up to maxlen digits. */
        size_t pos = 0;
        for (size_t len = 2; len <= maxlen; ++len) {   /* total digits */
            for (int first = 1; first <= 9; ++first) { /* first digit (a) */
                for (size_t k = 1; k <= len - 1; ++k) { /* length of first run */
                    size_t m = len - k;              /* length of second run */
                    for (int second = 0; second <= 9; ++second) { /* second digit (b) */
                        if (second == first) continue;
                        seq_t val = 0;
                        for (size_t i = 0; i < k; ++i)
                            val = val * 10 + first;
                        for (size_t i = 0; i < m; ++i)
                            val = val * 10 + second;
                        new_cache[pos++] = val;
                    }
                }
            }
        }

        /* Sort the temporary array. */
        qsort(new_cache, total_terms, sizeof(seq_t), seq_compare);

        /* Replace the old cache. */
        free(cache);
        cache     = new_cache;
        cache_len = (idx_t)total_terms;
    }

    /* Return the nth element (1‑based index). */
    return cache[n - 1];
}
