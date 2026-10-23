/*
 * A129207  Second sequence in solution to congruent number 5 problem.
 * https://oeis.org/A129207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(1) = 1 a(2) = -49 a(3) = -4799 for n ≥ 4: a(n) =
 *            (144*a(n‑1)*a(n‑3) + 2257*a(n‑2)*a(n‑2)) / a(n‑4) (equivalent to
 *            a(n+2)*a(n‑2) = 144*a(n+1)*a(n‑1) + 2257*a(n)²) and a(–n) =
 *            a(n). This recurrence generates OEIS A129207
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization */
static seq_t *seq_cache  = NULL;   /* computed a(i) values */
static idx_t  seq_cap    = 0;      /* allocated capacity */
static idx_t  seq_size   = 0;      /* next free index */

/* Grow the cache to hold at least up to index 'upto' */
static void ensure_cap(idx_t upto)
{
    if (upto + 1 > seq_cap) {
        idx_t new_cap = seq_cap ? (seq_cap << 1) : 8;
        while (new_cap <= upto) new_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap   = new_cap;
    }
}

/* Forthcoming terms of A129207 */
seq_t a(idx_t n)
{
    /* Initialise the cache with the first four terms if needed */
    if (seq_size == 0) {
        ensure_cap(3);                /* indices 0,1,2,3 */
        seq_cache[0] = (seq_t) 1;
        seq_cache[1] = (seq_t) 1;
        seq_cache[2] = (seq_t)-49;
        seq_cache[3] = (seq_t)-4799;
        seq_size     = 4;
    }

    /* Return already computed value */
    if (n < seq_size) return seq_cache[n];

    /* Compute forward up to the requested index */
    for (idx_t i = seq_size; i <= n; ++i) {
        seq_t num = 144 * seq_cache[i-1] * seq_cache[i-3]
                  + 2257 * seq_cache[i-2] * seq_cache[i-2];
        seq_t den = seq_cache[i-4];
        seq_t val = num / den;       /* exact division by definition */
        ensure_cap(i);
        seq_cache[i] = val;
    }

    return seq_cache[n];
}
