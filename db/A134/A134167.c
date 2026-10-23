/*
 * A134167  Complete list of solutions to y^2 = x^3 + 1025; sequence gives x
 *          values.
 * https://oeis.org/A134167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 7, i32 n <= 14, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integer solutions to y^2 = x^3 + 1025, return the n‑th
   smallest x (1‑indexed).  The set of solutions is finite; we scan
   x from –10 upwards, store discovered values, and stop when a
   predefined bound is exceeded.  The cache is static so calls can be
   made in any order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static cache of solutions */
    static seq_t *sol = NULL;
    static size_t cap = 0;
    static size_t sz  = 0;
    /* start scanning from the smallest known solution */
    static int64_t current_x = -10;
    static bool finished = false;
    /* bound beyond which we assume there are no further solutions */
    const idx_t MAX_BOUND = 2000000;   /* far larger than the last known */

    /* gather solutions until we have at least n of them */
    while (sz < n)
    {
        if (finished)
            break;

        if (current_x > (int64_t)MAX_BOUND)
        {
            finished = true;
            break;
        }

        /* compute x^3 + 1025; fits in int64_t for |x| <= MAX_BOUND */
        int64_t val = current_x * current_x * current_x + 1025;

        if (val >= 0)
        {
            seq_t y = floor_sqrt((seq_t)val);
            if (y * y == (seq_t)val)   /* perfect square found */
            {
                if (sz == cap)
                {
                    size_t new_cap = (cap == 0 ? 10 : cap * 2);
                    seq_t *tmp = (seq_t *)realloc(sol, new_cap * sizeof(seq_t));
                    if (!tmp && new_cap > 0)   /* allocation failed */
                        return (seq_t)0;
                    sol = tmp;
                    cap = new_cap;
                }
                sol[sz++] = (seq_t)current_x;
            }
        }

        current_x++;
    }

    /* if we couldn't find enough solutions, the sequence is finite and
       we return 0 for indices beyond its length. */
    if (sz < n)
        return (seq_t)0;

    return sol[n - 1];
}
