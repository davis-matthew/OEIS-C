/*
 * A080445  a(1) = 1, a(n) = smallest (nontrivial) multiple of a(n-1)
 *          containing n digits, a(n) not equal to 10*a(n-1).
 * https://oeis.org/A080445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1; for n>=2, a(n) = smallest nontrivial multiple of
 *            a(n-1) that has exactly n decimal digits, and is not equal to
 *            10*a(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;                /* out of range, not part of the sequence */

    static seq_t *cache   = NULL; /* a(1) stored at cache[0]... */
    static size_t cache_len = 0;  /* number of cached terms */

    /* Preserve old length before potential reallocation */
    size_t old_len = cache_len;

    if ((size_t)n > old_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        if (!tmp)
            return 0; /* allocation failure – abort */
        cache      = tmp;
        cache_len = new_len;
    }

    /* Compute any missing terms from (old_len + 1) up to n */
    for (idx_t i = old_len + 1; i <= n; ++i) {
        if (i == 1) {                 /* a(1) = 1 */
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t prev = cache[i - 2];    /* a(i-1) */
        seq_t result = 0;

        for (unsigned k = 2; ; ++k) {
            if (k == 10u)            /* skip the forbidden 10*a(i-1) */
                continue;

            seq_t cand = seq_mul(prev, (seq_t)k);
            if (num_digits(cand) == (int)i) {
                result = cand;
                break;
            }
        }
        cache[i - 1] = result;       /* store a(i) */
    }

    return cache[n - 1];
}
