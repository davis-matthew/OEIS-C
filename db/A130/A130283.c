/*
 * A130283  Integers n > 0 for which A130280(n) = 0, i.e., such that there is
 *          no integer m > 1 for which n(m^2 - 1) + 1 is a square.
 * https://oeis.org/A130283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the A130283 sequence:
   a(n) = the n-th integer >0 for which no integer m>1 satisfies
   n*(m^2-1)+1 is a perfect square.  The function returns
   a(idx_t n) with 1‑based indexing. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static bool has_solution_for_base(idx_t base)
{
    /* base > 1 */
    idx_t P = base * base - 1;          /* b^2 - 1, fits in idx_t */
    for (idx_t a = 1; a * a <= P; ++a) {
        if (P % a != 0)
            continue;
        idx_t d = a;                     /* smaller factor */
        idx_t e = P / a;                 /* larger factor  */
        /* (b*m - s) = d , (b*m + s) = e */
        if (((e - d) & 1) != 0)          /* difference must be even */
            continue;
        idx_t sum = e + d;
        if (sum % (2 * base) != 0)
            continue;
        idx_t m = sum / (2 * base);
        if (m > 1)
            return true;                /* solution found */
    }
    return false;                       /* no solution for this base */
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;
    static idx_t next_base = 2;          /* next base to test (b >= 2) */

    while (cache_size < n) {
        /* compute b^2 */
        seq_t b = (seq_t)next_base;
        seq_t square = b * b;

        if (!has_solution_for_base(next_base)) {
            /* store square in cache */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {            /* allocation failure */
                    exit(1);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = square;
        }
        ++next_base;
    }

    return cache[n - 1];
}
