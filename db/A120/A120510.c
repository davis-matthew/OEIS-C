/*
 * A120510  Generalized meta-Fibonacci sequence a(n) with parameters s=3 and
 *          k=4.
 * https://oeis.org/A120510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for 1 ≤ n ≤ 4, * a(n) = n‑3 for 5 ≤ n ≤ 7, * a(n) =
 *            a(n‑3‑a(n‑1)) + a(n‑4‑a(n‑2)) + a(n‑5‑a(n‑3)) + a(n‑6‑a(n‑4))
 *            for n > 7. * Offset: 1
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* 1‑based array of cached values */
static idx_t  cache_cap   = 0;    /* allocated capacity (number of slots) */
static idx_t  last_idx    = 0;    /* last computed index */

/* Return a(n) for n ≥ 1.  Undefined for n == 0 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1, there is no a(0) */

    /* Allocate or grow cache to hold indices up to n (1‑based). */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – best effort return 0. */
            return 0;
        }
        seq_cache = new_cache;
        cache_cap = new_cap;
        /* Initialise newly allocated slots (index 1..new_cap-1). */
        for (idx_t i = last_idx + 1; i < new_cap; ++i)
            seq_cache[i] = (seq_t)0;
    }

    /* If value already cached, return it. */
    if (n <= last_idx)
        return seq_cache[n];

    /* Compute missing values up to n. */
    for (idx_t i = last_idx + 1; i <= n; ++i) {
        if (i <= 4) {
            seq_cache[i] = (seq_t)1;
        } else if (i <= 7) {
            seq_cache[i] = (seq_t)(i - 3);
        } else {
            /* All indices k1..k4 are guaranteed ≥ 1 by the definition. */
            idx_t k1 = i - 3 - (idx_t)seq_cache[i - 1];
            idx_t k2 = i - 4 - (idx_t)seq_cache[i - 2];
            idx_t k3 = i - 5 - (idx_t)seq_cache[i - 3];
            idx_t k4 = i - 6 - (idx_t)seq_cache[i - 4];
            seq_cache[i] = seq_cache[k1] + seq_cache[k2]
                         + seq_cache[k3] + seq_cache[k4];
        }
    }

    last_idx = n;
    return seq_cache[n];
}
