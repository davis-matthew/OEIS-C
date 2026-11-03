/*
 * A161941  a(n) = ((4+sqrt(2))*(2+sqrt(2))^n + (4-sqrt(2))*(2-sqrt(2))^n)/4.
 * https://oeis.org/A161941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) Initial values: a(0) = 2, a(1) = 5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 951 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static size_t cap   = 0;
    static idx_t max_n  = 0;

    /* Allocate at least two slots on first call */
    if (memo == NULL) {
        size_t init_cap = (size_t)2;
        if (n + 1 > init_cap) init_cap = (size_t)(n + 1);

        memo = mem_new(seq_t, init_cap);
        if (memo == NULL) abort();

        memo[0] = (seq_t)2;
        memo[1] = (seq_t)5;
        cap   = init_cap;
        max_n = 1;
    }

    /* Return cached value if already computed */
    if (n <= max_n) {
        return memo[n];
    }

    /* Grow the table if necessary */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap > cap) {
        seq_t *tmp = mem_resize(seq_t, memo, new_cap);
        if (tmp == NULL) abort();
        memo = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        memo[i] = memo[i - 1] * (seq_t)4 - memo[i - 2] * (seq_t)2;
    }
    max_n = n;

    return memo[n];
}
