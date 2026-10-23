/*
 * A116537  a(1)=1. a(n) = number of positive integers that are < n, are
 *          coprime to n and do not occur among the first n-1 terms of the
 *          sequence.
 * https://oeis.org/A116537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = number of integers k < n such that gcd(k,n)=1
   and k has not appeared as a sequence term among a(1)..a(n-1). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Euclidean GCD for idx_t values. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Static state for memoization. */
    static seq_t *seq         = NULL;  /* sequence values, 1‑based */
    static bool *present      = NULL;  /* present[value] = true if value has appeared */
    static size_t seq_cap     = 0;     /* capacity of seq array */
    static size_t present_cap = 0;     /* capacity of present array */
    static idx_t computed_max = 0;     /* maximum n already computed */

    /* Grow seq array to hold indices up to n. */
    if (seq_cap <= (size_t)n)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        if (new_cap > seq_cap)
            memset(tmp + seq_cap, 0, (new_cap - seq_cap) * sizeof(seq_t));
        seq = tmp;
        seq_cap = new_cap;
    }

    /* Grow present array to index values up to n (since a(n) ≤ n‑1). */
    if (present_cap <= (size_t)n)
    {
        size_t new_cap = (size_t)n + 1;
        bool *tmp = (bool *)realloc(present, new_cap * sizeof(bool));
        if (!tmp) exit(1);
        if (new_cap > present_cap)
            memset(tmp + present_cap, 0, (new_cap - present_cap) * sizeof(bool));
        present = tmp;
        present_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t m = computed_max + 1; m <= n; ++m)
    {
        seq_t count;
        if (m == 1)
        {
            count = 1;
        }
        else
        {
            count = 0;
            for (idx_t k = 1; k < m; ++k)
            {
                if (gcd_idx(k, m) == 1 && !present[(size_t)k])
                    ++count;
            }
        }
        seq[m] = count;
        present[(size_t)count] = true;  /* mark that this value has appeared */
    }

    computed_max = n;
    return seq[n];
}
