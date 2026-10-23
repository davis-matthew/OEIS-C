/*
 * A116145  Squares that are equal to the sum of 5 consecutive cubes.
 * https://oeis.org/A116145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 6, u128 n <= 6, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116145: squares that are equal to the sum of five consecutive cubes.
   The terms are obtained from m^3+(m+1)^3+(m+2)^3+(m+3)^3+(m+4)^3 =
   5*(m+2)*(m^2+4*m+10).  The only integer values of m giving a square are
   m = -2, 0, 1, 25, 96, 118, producing the finite list
   {0, 100, 225, 99225, 4708900, 8643600}.  This code builds that list once
   and returns the nth element (n≥1).  For n beyond the list, it returns 0. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *solutions = NULL;  /* array of the found squares   */
static size_t   sol_len  = 0;     /* number of stored terms       */
static size_t   sol_cap  = 0;     /* current capacity of the array */
static bool built = false;       /* has the cache been built?    */

/* Build the cache once.  The search limit comfortably covers all known
   solutions (the largest m is 118). */
static void build_cache(void)
{
    if (built)
        return;

    const int64_t LIMIT = 2000;   /* high enough to include all solutions */
    const size_t INIT_CAP = 8;

    sol_cap = INIT_CAP;
    solutions = (seq_t *)malloc(sol_cap * sizeof(seq_t));
    if (!solutions)
        abort();

    sol_len = 0;

    for (int64_t m = -2; m <= LIMIT; ++m)
    {
        __int128 part1   = (__int128)m + 2;                        /* m+2   */
        __int128 part2   = (__int128)m * m + (__int128)4 * m + 10; /* m^2+4m+10 */
        __int128 cand128 = (__int128)5 * part1 * part2;            /* S(m)  */

        if (cand128 < 0)  /* negative values cannot be squares */
            continue;

        seq_t cand = (seq_t)cand128;

        seq_t r = floor_sqrt(cand);
        if (r * r != cand)   /* not a perfect square */
            continue;

        /* Store the square, resizing the array if needed. */
        if (sol_len == sol_cap) {
            sol_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(solutions, sol_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            solutions = tmp;
        }
        solutions[sol_len++] = cand;
    }

    built = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n<1 */

    if (!built)
        build_cache();

    if (n <= sol_len)
        return solutions[(size_t)n - 1];
    return 0;                /* for n beyond the known terms */
}
