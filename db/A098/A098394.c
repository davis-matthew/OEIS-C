/*
 * A098394  Next larger integer including the smallest digit of n.
 * https://oeis.org/A098394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_digit, min_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(1) = 1
 *   for n > 1, let d = smallest decimal digit of a(n‑1);
 *   a(n) = the smallest integer m > a(n‑1) that contains digit d
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_digit.h"
#include "../utils/min_digit.h"

seq_t a(idx_t n)
{
    /* cache the terms that have already been computed */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_len = 0;   /* number of computed terms in cache */

    if (n == 0)                     /* the sequence is 1‑based; return 0 for 0 */
        return 0;

    /* enlarge cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed */
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i < n; ++i) {
        if (i == 0) {          /* correspond to a(1) */
            cache[i] = 1;
            continue;
        }
        seq_t prev = cache[i - 1];          /* a(i) depends on a(i‑1) */
        int d = min_digit(prev);             /* smallest digit of previous term */
        seq_t m = prev + 1;                  /* start searching from next integer */
        while (!contains_digit(m, d))
            ++m;                             /* find the first with digit d */
        cache[i] = m;
    }

    cache_len = n;
    return cache[n - 1];
}
