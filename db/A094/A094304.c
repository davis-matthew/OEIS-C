/*
 * A094304  Sum of all possible sums formed from all but one of the previous
 *          terms, starting 1.
 * https://oeis.org/A094304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = (n‑2)! × (n‑2)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Result type and index type are provided by the build. */
seq_t a(idx_t n)
{
    /* Handle the fixed initial term. */
    if (n == 1) return (seq_t)1;

    /* For n ≥ 2 compute (n‑2)! × (n‑2).  The case n == 2 gives 0. */
    idx_t k      = n - 2;            /* k ≥ 0 */
    size_t need  = (size_t)k;        /* index in the factorial array */
    static seq_t *fact = NULL;       /* fact[i] = i! for i ≥ 0 */
    static size_t fact_cap = 0;      /* current allocated size of fact */

    /* Initialise the factorial array to hold 0! if it hasn't been done yet. */
    if (!fact) {
        fact = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!fact) return (seq_t)0;   /* allocation failure: return 0 */
        fact[0] = (seq_t)1;           /* 0! = 1 */
        fact_cap = 1;
    }

    /* Grow the array if we need a larger factorial. */
    if (need >= fact_cap) {
        size_t new_cap = need + 1;
        seq_t *tmp = (seq_t *)realloc(fact, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;   /* realloc failed: return 0 */
        fact = tmp;

        /* Compute the missing factorial entries. */
        for (size_t i = fact_cap; i <= need; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_cap = new_cap;
    }

    /* Return the required term. */
    return fact[need] * (seq_t)need;
}
