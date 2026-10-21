/*
 * A211804  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2w^3>=x^3+y^3.
 * https://oeis.org/A211804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with w,x,y∈{1..n} and
 *            2·w³ ≥ x³ + y³. The routine recomputes the total in O(n²) time
 *            with a single global cube table
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Global cache of cubes up to the largest n requested. */
static idx_t max_cube_n = 0;
static seq_t *cube_tbl  = NULL;

/* Grow the cube table to hold values up to 'need' (inclusive). */
static void ensure_cubes(idx_t need)
{
    if (need <= max_cube_n)
        return;

    /* Allocate new table. */
    seq_t *new_tbl = (seq_t *)malloc((need + 1) * sizeof(seq_t));
    if (!new_tbl)
        abort();               /* out of memory */

    /* Copy existing data. */
    for (idx_t i = 1; i <= max_cube_n; ++i)
        new_tbl[i] = cube_tbl[i];
    new_tbl[0] = 0;

    /* Free old storage. */
    free(cube_tbl);
    cube_tbl = new_tbl;

    /* Compute missing cubes. */
    for (idx_t i = max_cube_n + 1; i <= need; ++i)
        cube_tbl[i] = (seq_t)i * (seq_t)i * (seq_t)i;

    max_cube_n = need;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    ensure_cubes(n);

    seq_t total_triples = 0;

    /* For each w, count admissible (x,y). */
    for (idx_t w = 1; w <= n; ++w) {
        seq_t M = (seq_t)2 * cube_tbl[w];
        idx_t  y_max = n;
        seq_t  count_w = 0;

        for (idx_t x = 1; x <= n; ++x) {
            /* Decrease y_max until the sum fits. */
            while (y_max > 0 &&
                   cube_tbl[x] + cube_tbl[y_max] > M)
                --y_max;

            if (y_max == 0)
                break;          /* no further x will work either */

            count_w += (seq_t)y_max;   /* all y in [1..y_max] satisfy */
        }

        total_triples += count_w;
    }

    return total_triples;
}
