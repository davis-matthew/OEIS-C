/*
 * A072292  Number of proper powers b^d <= n (b > 1, d > 1).
 * https://oeis.org/A072292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A072292
    a(n) = number of proper powers b^d (b>1, d>1) that are ≤ n
    (offset 1 – a(1) = 0).  The function below lazily expands a
    cached boolean array of proper powers and a running sum array so
    that any call to a(n) (in any order) returns the correct result
    quickly. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static idx_t          max_n = 0;          /* largest n processed */
    static unsigned char *seen  = NULL;       /* seen[i] == 1 if i is a proper power */
    static seq_t         *counts = NULL;      /* counts[i] = a(i) */

    if (n == 0) return 0;   /* safety – OEIS offset is 1 */

    if (n > max_n) {
        idx_t old = max_n;
        max_n = n;

        /* ----- grow seen array ----- */
        unsigned char *tmp = (unsigned char *)realloc(seen,
                (size_t)(max_n + 1));
        seen = tmp;
        memset(seen + old + 1, 0, (size_t)(max_n - old));

        /* ----- mark all proper powers up to max_n ----- */
        seq_t       limit      = (seq_t)max_n;
        uint64_t    limit_u64  = (uint64_t)limit;
        idx_t       base_limit = (idx_t)floor_sqrt(limit);

        for (idx_t base = 2; base <= base_limit; ++base) {
            uint64_t val = (uint64_t)base * (uint64_t)base;   /* exponent 2 */
            while (val <= limit_u64) {
                seen[(idx_t)val] = 1;
                if (val > limit_u64 / (uint64_t)base) break;
                val *= (uint64_t)base;
            }
        }

        /* ----- expand and fill counts array ----- */
        seq_t *tmp_counts = (seq_t *)realloc(counts,
                (size_t)(max_n + 1) * sizeof(seq_t));
        counts = tmp_counts;

        if (old == 0) {
            counts[0] = 0;
            for (idx_t i = 1; i <= max_n; ++i)
                counts[i] = counts[i - 1] + (seq_t)seen[i];
        } else {
            for (idx_t i = old + 1; i <= max_n; ++i)
                counts[i] = counts[i - 1] + (seq_t)seen[i];
        }
    }

    return counts[n];
}
