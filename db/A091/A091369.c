/*
 * A091369  a(n) = Sum_{i=1..n} phi(i)*ceiling(n/i).
 * https://oeis.org/A091369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} phi(i) * ceil(n/i)
 * Range:     u8 n <= 18, u16 n <= 285, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/phi.h"

static seq_t *phi_cache = NULL;
static idx_t phi_cap   = 0;

/* Ensure phi_cache contains phi(i) for 1 <= i <= n */
static void ensure_phi(idx_t n)
{
    if (n <= phi_cap) return;

    idx_t   new_cap = n;
    seq_t * new_arr = (seq_t *)realloc(phi_cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_arr) return;        /* in practice the build will not run out of memory */

    /* compute new entries */
    for (idx_t i = phi_cap + 1; i <= new_cap; ++i)
        new_arr[i] = phi(i);      /* phi takes idx_t and returns seq_t */

    phi_cache = new_arr;
    phi_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1; undefined for 0 */
    ensure_phi(n);

    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t ceil = (n + i - 1) / i;     /* ceil(n/i) */
        seq_t term = phi_cache[i] * (seq_t)ceil;
        res += term;
    }
    return res;
}
