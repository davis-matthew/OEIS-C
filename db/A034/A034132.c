/*
 * A034132  Decimal part of cube root of a(n) starts with 6: first term of
 *          runs.
 * https://oeis.org/A034132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A034132: first terms of runs where the decimal part of the cube
 * root of a(n) starts with 6.  The property for an integer m is
 *
 *   floor(∛m) = r   and   (10·r + 6)³ ≤ 1000·m < (10·r + 7)³ .
 *
 * This is equivalent to 0.6 ≤ ∛m – r < 0.7, i.e., the first decimal digit of
 * ∛m is 6.  The implementation searches integers, remembers the first
 * integer of each run, and caches the results for subsequent calls. */
 
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_cube_root.h"  /* floor_cube_root(seq_t) */

/*  Cache for the values of the sequence  */
static seq_t *run_cache      = NULL;   /* array of found terms   */
static size_t run_cap        = 0;      /* current capacity       */
static size_t run_len        = 0;      /* number of terms stored */
static idx_t  last_checked   = 0;      /* last integer tested   */
static bool   prev_good      = false;  /* previous integer sat? */

/*  Grow the cache when needed  */
static void grow_cache(size_t needed)
{
    if (needed <= run_cap)
        return;
    size_t new_cap = run_cap ? run_cap * 2 : 16;
    while (needed > new_cap)
        new_cap *= 2;
    run_cache = (seq_t *)realloc(run_cache, new_cap * sizeof(seq_t));
    run_cap   = new_cap;
}

/*  Test whether the decimal part of the cube root of n starts with 6  */
static bool cube_frac_start6(seq_t n)
{
    /* Integer part of cube root */
    seq_t r = floor_cube_root(n);

    /* (10·r + 6)³  and  (10·r + 7)³  (both 128‑bit or wider) */
    seq_t base = r * (seq_t)10 + (seq_t)6;
    seq_t left = base * base * base;

    base = r * (seq_t)10 + (seq_t)7;
    seq_t right = base * base * base;

    /* 1000 · n  for the integer comparison */
    seq_t m = n * (seq_t)1000;

    return (left <= m) && (m < right);
}

/*  Main entry point  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined for n=0 */

    /* Generate terms until we have at least n of them */
    while (run_len < (size_t)n)
    {
        ++last_checked;                     /* check next integer   */
        bool good = cube_frac_start6(last_checked);

        /* If good and previous integer was not good, this is the start
         * of a new run – record it. */
        if (good && !prev_good)
        {
            grow_cache(run_len + 1);
            run_cache[run_len++] = last_checked;
        }
        prev_good = good;                   /* remember for next step */
    }

    return run_cache[(size_t)(n - 1)];
}
