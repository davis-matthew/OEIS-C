/*
 * A164633  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(0)=1, a(1)=20, a(2)=380, a(3)=7220, a(4)=137180,
 *           a(5)=2606420, a(6)=49521980.
 * For n = 7:  a(7) = 18·a(6)+⋯+18·a(1) – 171·a(0) + 1.
 * For n ≥ 8:  a(n) = 18·a(n‑1)+18·a(n‑2)+18·a(n‑3)+18·a(n‑4)+18·a(n‑5)+18·a(n‑6)
 *             – 171·a(n‑7).
 */

#include <stdlib.h>

/* static cache of already computed terms */
static seq_t *cache   = NULL;   /* array holding terms a(0)...a(cache_size-1)   */
static idx_t  csz     = 0;       /* number of entries currently stored          */
static idx_t  cap     = 0;       /* current allocated capacity                  */

/* enlarge allocated storage to hold at least needed entries */
static void
ensure_cap(idx_t needed)
{
    if (needed <= cap) return;
    idx_t new_cap = (cap == 0) ? 8 : cap;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();             /* memory failure → terminate safely         */
    cache = tmp;
    cap   = new_cap;
}

/* fill cache up to at least index n (inclusive) */
static void
compute_upto(idx_t n)
{
    /* make sure the cache is large enough */
    ensure_cap(n + 1);

    /* initialise the first seven terms if not yet done */
    if (csz < 7) {
        const seq_t init[7] = {
            (seq_t)1,
            (seq_t)20,
            (seq_t)380,
            (seq_t)7220,
            (seq_t)137180,
            (seq_t)2606420,
            (seq_t)49521980
        };
        for (idx_t i = csz; i < 7 && i < cap; ++i)
            cache[i] = init[i];
        csz = 7;
    }

    /* compute missing terms in order */
    for (idx_t i = csz; i <= n; ++i) {
        if (i == 7) {                                   /* special = +1 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 6; ++k)
                sum += (seq_t)18 * cache[i - k];
            sum -= (seq_t)171 * cache[0];
            sum += (seq_t)1;
            cache[i] = sum;
        }
        else if (i > 7) {                               /* regular recurrence */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 6; ++k)
                sum += (seq_t)18 * cache[i - k];
            sum -= (seq_t)171 * cache[i - 7];
            cache[i] = sum;
        }
        /* i < 7 already initialised – nothing to do */
    }

    csz = n + 1;   /* now terms 0 … n are valid */
}

seq_t a(idx_t n)
{
    if (n < csz) return cache[n];
    compute_upto(n);
    return cache[n];
}
