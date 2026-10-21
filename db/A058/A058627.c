/*
 * A058627  Numbers n such that phi(n) = product of the digits of n.
 * https://oeis.org/A058627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   digit_product, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers n such that φ(n) equals the product of the digits of n. */

#include <stdlib.h>
#include <stdbool.h>

/* Utility functions provided by the build */
#include "../utils/phi.h"             /* static seq_t phi(idx_t) */
#include "../utils/digit_product.h"   /* static seq_t digit_product(seq_t) */

/* Cached solutions up to the largest n requested so far. */
static seq_t *solutions = NULL;
static size_t sol_len  = 0;   /* number of solutions stored */
static size_t sol_cap  = 0;   /* capacity of solutions array */
static idx_t  last_checked = 1;   /* next integer to test */

/* Append a solution, resizing if necessary. */
static void push_solution(seq_t val)
{
    if (sol_len == sol_cap) {
        sol_cap = sol_cap ? sol_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(solutions, sol_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        solutions = tmp;
    }
    solutions[sol_len++] = val;
}

/* Compute solutions up to the required index. */
static void compute_until(idx_t want)
{
    /* Upper bound set to find all known terms (up to 68796). */
    const idx_t MAX_SEARCH = 2000000ULL;
    for (; last_checked <= MAX_SEARCH && sol_len < (size_t)want; ++last_checked) {
        seq_t m = (seq_t)last_checked;
        if (phi(last_checked) == digit_product(m))
            push_solution(m);
    }
}

/* Return the nth entry of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1 */
        return 0;

    if (!solutions) {    /* first call: initialise */
        sol_cap = 16;
        solutions = (seq_t *)malloc(sol_cap * sizeof(seq_t));
        if (!solutions) exit(EXIT_FAILURE);
    }

    if (sol_len < (size_t)n)
        compute_until(n);

    return (sol_len >= (size_t)n) ? solutions[n - 1] : 0;
}
