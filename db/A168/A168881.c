/*
 * A168881  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence with a special term for n = 22 a(0) … a(21) are given
 *            in the OEIS entry. For n >= 23: a(n) = 11*a(n-1) - 65*a(n-21) +
 *            55*a(n-22) For n = 22 the right‑hand side of the generating
 *            function contributes an additional -1, so: a(22) = 11*a(21) -
 *            65*a(1) + 55*a(0) - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_from_str.h"

static const char *const init_vals[22] = {
    "1",
    "12",
    "132",
    "1452",
    "15972",
    "175692",
    "1932612",
    "21258732",
    "233846052",
    "2572306572",
    "28295372292",
    "311249095212",
    "3423740047332",
    "37661140520652",
    "414272545727172",
    "4556998002998892",
    "50126978032987812",
    "551396758362865932",
    "6065364341991525252",
    "66719007761906777772",
    "733909085380974555492",
    "8072999939190720110346"
};

seq_t a(idx_t n)
{
    /* Simple memoisation with a growing static array */
    static seq_t *cache = NULL;
    static size_t len   = 0; /* number of valid entries stored */
    static size_t cap   = 0; /* allocated capacity */

    /* Expand cache if necessary */
    if ((size_t)n >= len) {
        size_t new_cap = (cap == 0) ? 22 : cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cap   = new_cap;

        /* Initialise first 22 terms once */
        if (len == 0) {
            for (size_t i = 0; i < 22; ++i)
                cache[i] = seq_from_str(init_vals[i]);
            len = 22;
        }

        /* Compute new terms up to index n */
        for (size_t i = len; i <= (size_t)n; ++i) {
            if (i < 22) {
                /* already initialised */
                continue;
            } else if (i == 22) {
                /* special inhomogeneous term corresponding to coefficient -1 */
                seq_t val = 11 * cache[i - 1];
                val -= 65 * cache[i - 21];
                val += 55 * cache[i - 22];
                val -= 1;                         /* the extra -1 from numerator */
                cache[i] = val;
            } else {
                seq_t val = 11 * cache[i - 1];
                val -= 65 * cache[i - 21];
                val += 55 * cache[i - 22];
                cache[i] = val;
            }
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
