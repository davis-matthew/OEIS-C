/*
 * A215851  Number of simple labeled graphs on n nodes with exactly 1
 *          connected component that is a tree or a cycle.
 * https://oeis.org/A215851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(n)=n^(n-2)+(n-1)!/2 for n>2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 146 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

static seq_t tree_component_count(idx_t n)
{
    /* n^(n-2) */
    return ipow((seq_t)n, (seq_t)(n - 2));
}

static seq_t cycle_component_count(idx_t n)
{
    /* (n-1)! / 2 */
    return factorial(n - 1) / 2;
}

seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return 1;
    return tree_component_count(n) + cycle_component_count(n);
}
