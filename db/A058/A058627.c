/*
 * A058627  Numbers n such that phi(n) = product of the digits of n.
 * https://oeis.org/A058627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-03)
 * Helpers:   digit_product, mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers n such that φ(n) equals the product of the digits of n. */

#include <stdlib.h>
#include <stdbool.h>

/* Utility functions provided by the build */
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/totient.h"
#include "../utils/mem.h"

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
        seq_t *tmp = mem_resize(seq_t, solutions, sol_cap);
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
        if (totient(last_checked) == digit_product(m))
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
        solutions = mem_new(seq_t, sol_cap);
        if (!solutions) exit(EXIT_FAILURE);
    }

    if (sol_len < (size_t)n)
        compute_until(n);

    return (sol_len >= (size_t)n) ? solutions[n - 1] : 0;
}
