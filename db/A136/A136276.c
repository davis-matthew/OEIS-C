/*
 * A136276  Consider pairs of nonnegative integers (m,k) such that 2^2 + 4^2 +
 *          6^2 + ... + (2m)^2 = k(k+1); sequence gives k values.
 * https://oeis.org/A136276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) returns k such that 2^2+4^2+...+(2m)^2 = k(k+1). The
 *            sequence lists the k-values of all integer solutions (m,k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm searches for solutions by scanning m = 0,1,2,...,
   computing the sum of squares S = 2/3 * m(m+1)(2m+1) and checking
   whether S equals a pronic number k(k+1).  This is equivalent
   to testing if D = 1 + 4*S is a perfect square.  All found k-values
   are stored in a static cache so that repeated calls to a(idx_t n)
   are fast.  As the equation admits only finitely many solutions,
   the search stops once the desired index has been reached. */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

#define MAX_M 1000000UL   /* safe upper bound for the search */

static seq_t *solutions = NULL;   /* cache of found k-values */
static size_t sol_cap   = 0;      /* size of allocated cache */
static size_t sol_len   = 0;      /* number of stored solutions  */
static idx_t  next_m    = 0;      /* next m to examine */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_sol_cap(size_t needed)
{
    if (needed <= sol_cap)
        return;
    size_t new_cap = sol_cap ? sol_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    if (solutions == NULL)
        solutions = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        solutions = (seq_t *)realloc(solutions, new_cap * sizeof(seq_t));
    sol_cap = new_cap;
}

/* Public function returning the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for 0, but safe */

    /* Continue searching until we have at least n solutions or run out. */
    while (sol_len < (size_t)n && next_m <= MAX_M)
    {
        seq_t m = (seq_t)next_m;
        /* S = 2/3 * m(m+1)(2m+1) */
        seq_t S = ((((seq_t)2 * m) * (m + (seq_t)1)) *
                   ((seq_t)2 * m + (seq_t)1)) / (seq_t)3;
        /* D = 1 + 4*S */
        seq_t D = ((seq_t)1) + ((seq_t)4 * S);
        seq_t sqrtD = floor_sqrt(D);

        if ( (sqrtD * sqrtD == D) && ((sqrtD & (seq_t)1) == (seq_t)1) )
        {
            seq_t k = (sqrtD - (seq_t)1) >> 1;   /* (sqrtD-1)/2 */
            ensure_sol_cap(sol_len + 1);
            solutions[sol_len++] = k;
        }
        next_m++;
    }

    if (sol_len >= (size_t)n)
        return solutions[n - 1];
    else
        return 0;   /* for indices beyond the known solutions */
}
