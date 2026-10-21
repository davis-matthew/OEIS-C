/*
 * A070961  a(n) = Card{ k<=n | bigomega(k) != bigomega(n) }.
 * https://oeis.org/A070961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{k ≤ n | bigomega(k) ≠ bigomega(n)}. Input index is
 *            1‑based (a(1)=0). The function works for calls in any order and
 *            uses a simple incremental sieve of bigomega values up to the
 *            greatest n requested so far. It relies on the provided util
 *            function bigomega(idx_t)
 * Range:     u8 n <= 279, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/bigomega.h"

static seq_t *omega_counts = NULL;   /* number of occurrences of each bigomega */
static size_t   counts_cap = 0;       /* current size of omega_counts array */
static idx_t    precomp_limit = 0;    /* highest k that has been processed */

/* Ensure that all integers 1..n are processed and counted. */
static void _ensure_limit(idx_t n)
{
    if (n <= precomp_limit)
        return;

    for (idx_t k = precomp_limit + 1; k <= n; ++k) {
        seq_t om = bigomega(k);                 /* bigomega(k) */
        idx_t om_val = (idx_t)om;               /* safe small value */

        /* Make sure the counts array can hold index om_val. */
        if (om_val >= (idx_t)counts_cap) {
            size_t new_cap = (size_t)(om_val + 1);
            /* Expand the array to fit the new max index. */
            seq_t *tmp = (seq_t *)realloc(omega_counts,
                                          new_cap * sizeof(*tmp));
            if (!tmp) {
                /* Allocation failed – abort to avoid undefined behaviour. */
                abort();
            }
            /* Initialize newly allocated entries to zero. */
            for (size_t i = counts_cap; i < new_cap; ++i)
                tmp[i] = 0;
            omega_counts = tmp;
            counts_cap    = new_cap;
        }
        omega_counts[om_val]++;                  /* increment count of this ω */
    }
    precomp_limit = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* For n ≥ 1.  Undefined for n == 0, but calls with 0 can be
       assumed not to happen under the problem specification. */
    _ensure_limit(n);

    seq_t om_n = bigomega(n);
    idx_t om_val = (idx_t)om_n;

    seq_t same_cnt = (om_val < (idx_t)counts_cap) ? omega_counts[om_val]
                                                 : (seq_t)0;

    return (seq_t)n - same_cnt;
}
