/*
 * A034128  Decimal part of cube root of a(n) starts with 2: first term of
 *          runs.
 * https://oeis.org/A034128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the first element of each run of consecutive
   integers k for which the first digit after the decimal point of ∛k
   equals 2.  The sequence lists the starting value of each such run. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_cube_root.h"

/* Cache of run starts */
static seq_t *run_starts = NULL;
static size_t run_count = 0;
static size_t run_cap   = 0;

/* Last integer examined and whether it satisfied the property */
static seq_t last_checked = 0;
static bool  prev_prop = false;

/* Append a new run start to the cache, enlarging the array if needed */
static void append_run(seq_t val)
{
    if (run_count >= run_cap) {
        size_t new_cap = run_cap ? run_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(run_starts, new_cap * sizeof(seq_t));
        run_starts = tmp;
        run_cap   = new_cap;
    }
    run_starts[run_count++] = val;
}

/* Test whether integer m has first cube‑root fractional digit equal to 2.
   All computations use seq_t only. */
static bool first_digit_2(seq_t m)
{
    seq_t r = floor_cube_root(m);               /* floor of ∛m */

    /* Compute L = 8 * (5*r + 1)^3 */
    seq_t a = r * (seq_t)5 + (seq_t)1;         /* 5*r + 1 */
    a = a * a * a;                             /* (5*r + 1)^3 */
    seq_t L = a * (seq_t)8;                    /* times 8 */

    /* Compute R = (10*r + 3)^3 */
    seq_t b = r * (seq_t)10 + (seq_t)3;        /* 10*r + 3 */
    b = b * b * b;                             /* (10*r + 3)^3 */

    /* m * 1000 */
    seq_t m1000 = m * (seq_t)1000;

    return (L <= m1000) && (m1000 < b);
}

seq_t a(idx_t n)
{
    /* n >= 1, offset is 1 */
    size_t target = (size_t)(n - 1); /* index in cache */

    /* Generate runs until we have at least target+1 of them */
    while (run_count <= target) {
        last_checked++;
        seq_t m = last_checked;
        bool cur_prop = first_digit_2(m);

        if (cur_prop && !prev_prop) {
            append_run(m);            /* start of a new run */
        }
        prev_prop = cur_prop;          /* update for next iteration */
    }

    return run_starts[target];
}
