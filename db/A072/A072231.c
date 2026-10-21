/*
 * A072231  a(n) = floor(n^2/A005185(n-1)), where A005185 is Hofstadter's
 *          Q-sequence.
 * https://oeis.org/A072231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2 / A005185(n))
 * Range:     u8 n <= 15, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A005185: Hofstadter Q‑sequence, defined by Q(1)=Q(2)=1,
 *           Q(n)=Q(n‑Q(n‑1))+Q(n‑Q(n‑2)) for n>2. */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for Q(n).  Index 1…n; element 0 unused. */
static seq_t *q_cache   = NULL;   /* allocated array */
static idx_t  q_cap     = 0;      /* current allocated capacity */
static idx_t  q_max     = 0;      /* highest n for which Q(n) is computed */

/* Ensure q_cache contains Q(1)…Q(n).  Works for any order of calls. */
static void ensure_q(idx_t n)
{
    if (q_max >= n) return;                    /* already computed */

    /* Grow the cache if needed. */
    if (q_cap <= n) {
        idx_t new_cap = q_cap ? q_cap << 1 : 8;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(q_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */
        q_cache = tmp;
        for (idx_t i = q_cap; i < new_cap; ++i) q_cache[i] = 0;
        q_cap = new_cap;
    }

    /* Compute missing Q values incrementally. */
    for (idx_t i = q_max + 1; i <= n; ++i) {
        if (i == 1 || i == 2) {
            q_cache[i] = 1;
        } else {
            idx_t idx1 = i - (idx_t)q_cache[i - 1];
            idx_t idx2 = i - (idx_t)q_cache[i - 2];
            q_cache[i] = q_cache[idx1] + q_cache[idx2];
        }
    }
    q_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, guard against misuse */

    ensure_q(n);                     /* compute Q(n) */

    seq_t denom = q_cache[n];        /* Q(n) */
    seq_t nsq   = (seq_t)n * (seq_t)n;
    return nsq / denom;              /* floor division on unsigned types */
}
