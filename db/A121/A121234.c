/*
 * A121234  Nonnegative integers k such that k*(k+1)*(k+2)+1 is a square.
 * https://oeis.org/A121234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   append_candidate, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A121234:
   a(n) is the n-th nonnegative integer k such that k*(k+1)*(k+2)+1 is a perfect square.
*/

#include <stddef.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t x) -> seq_t */
#include "../utils/append_candidate.h" /* append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val) */

static seq_t *solutions = NULL;      /* dynamic array of found k values */
static size_t sol_len  = 0;          /* number of stored solutions */
static size_t sol_cap  = 0;          /* current capacity of the array */
static idx_t  checked  = 0;          /* last k checked */
static int    finished = 0;          /* set to 1 when no further solutions exist */

/* Check whether a given k satisfies the defining condition. */
static bool is_solution(idx_t k)
{
    seq_t nk = (seq_t)k;
    seq_t prod = nk * (nk + 1) * (nk + 2) + 1;   /* k*(k+1)*(k+2)+1 */
    seq_t s = floor_sqrt(prod);
    return s * s == prod;
}

/* Extend the solutions array until it contains at least n terms
   or until we determine that no more solutions exist. */
static void grow_solutions(idx_t n)
{
    while (n > sol_len && !finished) {
        if (is_solution(checked)) {
            append_candidate(&solutions, &sol_len, &sol_cap, (seq_t)checked);
            /* The sequence is known to have only 4 non‑negative solutions,
               so stop once we have them all. */
            if (sol_len >= 4) finished = 1;
        }
        ++checked;
        /* Guard against infinite loops for an unexpected input. */
        if (checked > (idx_t)1000000) { finished = 1; break; }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n=0, return 0 */
    grow_solutions(n);
    if (n <= sol_len) return solutions[n - 1];
    return 0;                           /* sequence terminates after the last solution */
}
