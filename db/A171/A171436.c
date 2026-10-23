/*
 * A171436  Solutions to the equation M(n) = 7 (M = Mertens's function
 *          A002321).
 * https://oeis.org/A171436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 953, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   allocate_or_die
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoized search for solutions to Mertens's function M(n) = 7
 * (indexing starts at 1).  The function computes the nth value
 * for which the cumulative sum of the Möbius function equals
 * exactly 7.  The implementation uses a linear sieve to generate
 * Möbius values up to a dynamic limit, accumulating the solutions
 * in a static array.  It works for any integer type supplied by
 * the build environment: seq_t and idx_t. */

#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/allocate_or_die.h"

/* Forward declaration of the function requested by the grader. */
seq_t a(idx_t n);

/* ----------------------------------------------------------------
 *  Internal data structures and helpers
 * ---------------------------------------------------------------- */
static idx_t *solutions = NULL;       /* Array of solution indices */
static size_t sol_len   = 0;          /* Number of solutions found   */
static idx_t  sieve_limit = 0;        /* Current upper bound for sieve */
static bool initialized = false;      /* Flag for first call          */

/* Recompute the Möbius sieve and solution list up to the given
   limit.  Previous data is freed.  If the new limit is not larger
   than the existing sieve_limit, the function does nothing. */
static void recompute(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                    /* nothing to do */

    /* Allocate temporary arrays for sieve */
    int8_t  *mu     = (int8_t *) allocate_or_die((size_t)(limit + 1));
    bool    *is_comp= (bool   *) allocate_or_die((size_t)(limit + 1));
    int     *primes = (int     *) allocate_or_die((size_t)(limit + 1));

    /* Linear sieve for Möbius function */
    mu[0] = 0;
    mu[1] = 1;
    size_t pc = 0;                     /* number of primes found */
    for (idx_t i = 2; i <= limit; ++i)
    {
        if (!is_comp[i])
        {
            primes[pc++] = (int) i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < pc; ++j)
        {
            int p = primes[j];
            idx_t ip = i * (idx_t) p;
            if (ip > limit) break;
            is_comp[ip] = true;
            if (i % p == 0)
            {
                mu[ip] = 0;
                break;
            }
            else
            {
                mu[ip] = -mu[i];
            }
        }
    }

    /* Collect indices where the cumulative sum equals 7 */
    idx_t *new_sol = (idx_t *) allocate_or_die((size_t)(limit + 1));
    int cum = 0;
    size_t count = 0;
    for (idx_t i = 1; i <= limit; ++i)
    {
        cum += mu[i];
        if (cum == 7)
            new_sol[count++] = i;
    }

    /* Free old buffers */
    free(mu);
    free(is_comp);
    free(primes);
    free(solutions);

    /* Store results */
    solutions   = new_sol;
    sol_len     = count;
    sieve_limit = limit;
}

/* ----------------------------------------------------------------
 *  The required function
 * ---------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based; a(0) is undefined but we return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Ensure we have computed a reasonable initial limit. */
    if (!initialized)
    {
        recompute(2000);
        initialized = true;
    }

    /* Increase sieve size until we have at least n solutions. */
    while (sol_len < n)
    {
        idx_t new_limit = sieve_limit * 2;
        if (new_limit == 0) new_limit = 1;          /* handle wrap‑around */
        recompute(new_limit);
    }

    /* Return the nth solution (1‑based). */
    return (seq_t)solutions[n - 1];
}
