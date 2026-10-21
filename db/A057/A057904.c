/*
 * A057904  Positive integers that are not the sum of exactly three positive
 *          cubes.
 * https://oeis.org/A057904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th positive integer that is NOT representable as
   the sum of three positive integer cubes (OEIS A057904).  A
   zero‑based lookup is not required; a(1) equals the first term in
   the OEIS listing. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_cube_root.h"   /* floor_cube_root(seq_t) */

static seq_t *missing_vals = NULL;   /* 1‑based array of missing numbers */
static size_t missing_count = 0;     /* count of stored missing numbers */

static bool *repr = NULL;            /* representation bitmap */
static size_t repr_limit = 0;        /* current maximum sum represented */

/* Grows the internal representation bitmap and missing list until
   at least `n` missing numbers are available. */
static void ensure_capacity(idx_t n)
{
    if (missing_count >= (size_t)n)
        return;

    /* Start at a modest limit and double until enough terms are found. */
    size_t limit = (repr_limit == 0) ? 64UL : repr_limit * 2;
    while (missing_count < (size_t)n) {
        bool *new_repr = (bool *)calloc(limit + 1, sizeof(bool));
        if (!new_repr)
            abort();   /* fatal allocation error */

        seq_t max_a = floor_cube_root((seq_t)limit);
        for (idx_t a = 1; a <= max_a; ++a) {
            seq_t a3 = ((seq_t)a) * a * a;
            for (idx_t b = 1; b <= max_a; ++b) {
                seq_t b3 = ((seq_t)b) * b * b;
                seq_t ab3 = a3 + b3;
                for (idx_t c = 1; c <= max_a; ++c) {
                    seq_t sum = ab3 + ((seq_t)c) * c * c;
                    if (sum <= limit)
                        new_repr[(size_t)sum] = true;
                    else
                        break;   /* larger c will only increase the sum */
                }
            }
        }

        /* Allocate space for the missing numbers up to 'limit'. */
        seq_t *new_missing = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
        if (!new_missing)
            abort();

        size_t cnt = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            if (!new_repr[(size_t)i])
                new_missing[cnt++] = i;
        }

        /* Replace the old data. */
        free(repr);
        free(missing_vals);
        repr     = new_repr;
        repr_limit = limit;
        missing_vals = new_missing;
        missing_count = cnt;

        /* Increase the limit for the next iteration if needed. */
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; n==0 is undefined – return 0 */
        return 0;

    ensure_capacity(n);
    /* n is 1‑based; missing_vals is 0‑based */
    return missing_vals[(size_t)n - 1];
}
