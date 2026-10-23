/*
 * A048254  Numbers whose sum of divisors is 6^4 = 1296.
 * https://oeis.org/A048254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   numbers n such that σ(n) = 1296 (6⁴). The sequence is finite
 *            because σ(n) ≥ n+1, so n ≤ 1295. This implementation scans
 *            1..1295 once, stores every solution in a static array, and
 *            returns the n‑th term requested
 * Range:     u8 none, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sigma_of.h"   /* sigma_of(idx_t n) -> seq_t */

static seq_t *solutions = NULL;
static size_t solutions_len = 0;
static size_t solutions_cap = 0;

/* Build the list of all n ≤ 1295 with σ(n) == 1296. */
static void build_solutions(void)
{
    static int built = 0;
    if (built) return;
    built = 1;

    solutions_cap = 32;
    solutions = (seq_t *)malloc(solutions_cap * sizeof(seq_t));
    if (!solutions) exit(1);

    for (idx_t n = 1; n <= 1295; ++n) {
        if (sigma_of(n) == (seq_t)1296) {
            if (solutions_len == solutions_cap) {
                solutions_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(solutions, solutions_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                solutions = tmp;
            }
            solutions[solutions_len++] = (seq_t)n;
        }
    }
}

/* External linkage – the required function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for index 0 */
    build_solutions();
    if (n - 1 < solutions_len) return solutions[n - 1];
    return 0; /* beyond known terms → 0 */
}
