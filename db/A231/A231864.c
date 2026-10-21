/*
 * A231864  Partial sums of the second power of arithmetic derivative function
 *          A003415.
 * https://oeis.org/A231864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (i')^2, where i' is the arithmetic
 *            derivative of i (i' = Σ a_j * i / p_j for i = Π p_j^{a_j})
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static cache for partial sums.  prefix[i] holds a(i) for i≥1. */
static seq_t *prefix = NULL;   /* array indices use idx_t; prefix[0] unused */
static idx_t prefix_cap = 0;   /* current array capacity (indices 0..cap-1) */

/* Ensure the cache is large enough to hold a(n). */
static void ensure_prefix(idx_t n)
{
    if (n + 1 <= prefix_cap) return;            /* already big enough */

    size_t new_cap = n + 1;                      /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(prefix, new_cap * sizeof(seq_t));
    if (!tmp) return;                           /* allocation failed */
    /* zero-initialize the new entries */
    for (idx_t i = prefix_cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;
    prefix = tmp;
    prefix_cap = new_cap;
}

/* Computes the sequence value a(n).  Can be called in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* offset starts at 1 */

    ensure_prefix(n);
    if (!prefix) return (seq_t)0;                /* memory allocation failed */

    /* Compute all missing values up to n. */
    for (idx_t i = 1; i <= n; ++i) {
        /* If we already computed this index (non‑zero or i==1),
           we simply skip; for i==1 the value is 0 and will be recomputed.
           Recomputing does no harm but keeps the logic simple. */
        seq_t deriv = 0;
        if (i > 1) {
            idx_t temp = i;
            /* Loop over prime factors of i. */
            for (idx_t p = 2; p * p <= temp; p += (p == 2 ? 1 : 2)) {
                if (temp % p == 0) {
                    idx_t ex = 0;
                    while (temp % p == 0) {
                        temp /= p;
                        ++ex;
                    }
                    deriv += (seq_t)ex * (seq_t)(i / p);
                }
            }
            if (temp > 1) { /* remaining prime factor */
                deriv += (seq_t)(i / temp);
            }
        }
        seq_t deriv_sq = deriv * deriv;
        seq_t prev = (i == 1) ? (seq_t)0 : prefix[i - 1];
        prefix[i] = prev + deriv_sq;
    }

    return prefix[n];
}
