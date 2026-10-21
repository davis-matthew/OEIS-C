/*
 * A088687  Numbers that can be represented as j^4 + k^4, with 0 < j < k, in
 *          exactly one way.
 * https://oeis.org/A088687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 none, u64 n <= 4500, u128 n <= 4500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A088687 – Numbers that can be written as j^4 + k^4
 *  (with 0 < j < k) in exactly one way.
 *
 *  The routine generates all pairs (j,k) with 1 ≤ j < k ≤ B, counts
 *  how many times each sum occurs, and keeps only the sums that occur
 *  once.  B is increased until at least n terms have been collected.
 *  Results are cached so subsequent calls only grow the bound when
 *  needed.  The algorithm is O(B^2 log B) with B ≈ O(n^(1/2)),
 *  which is fast enough for n up to ~10000 on the given machines.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Comparator for qsort, required for all seq_t variants. */
static int cmp_seq_t(const void *p, const void *q)
{
    const seq_t a = *(const seq_t *)p;
    const seq_t b = *(const seq_t *)q;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /*  Memoised cache of unique sums */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* number of cached unique sums */
    static idx_t B = 1;             /* current maximum j/k value used */

    if (n == 0) return 0;  /* undefined, but harmless */

    /*  Grow the cache until it contains at least n terms.  */
    while (cache_len < (size_t)n)
    {
        /*  Choose a new bound – a growth factor that keeps the work
         *  modest while discovering many new terms.  The step size is
         *  adjustable if needed.  */
        const idx_t new_B = B + 256;

        /*  Pre‑compute fourth powers up to new_B.  */
        seq_t *pow4 = (seq_t *)malloc((size_t)(new_B + 1) * sizeof(seq_t));
        if (!pow4) return 0;   /* memory failure – unlikely in tests */

        for (idx_t i = 1; i <= new_B; ++i)
        {
            seq_t p = (seq_t)i;
            p *= p;   /* i^2          */
            p *= p;   /* i^4          */
            pow4[i] = p;
        }

        /*  Generate all sums j^4 + k^4 with 0 < j < k ≤ new_B.  */
        const size_t pair_cnt = (size_t)new_B * (new_B - 1) / 2;
        seq_t *all = (seq_t *)malloc(pair_cnt * sizeof(seq_t));
        if (!all) { free(pow4); return 0; }

        size_t idx = 0;
        for (idx_t j = 1; j < new_B; ++j)
        {
            for (idx_t k = j + 1; k <= new_B; ++k)
                all[idx++] = pow4[j] + pow4[k];
        }

        /*  Sort the sums so equal values become adjacent.  */
        qsort(all, pair_cnt, sizeof(seq_t), cmp_seq_t);

        /*  Extract sums that appear exactly once.  */
        seq_t *uniq = (seq_t *)malloc(pair_cnt * sizeof(seq_t));
        if (!uniq) { free(all); free(pow4); return 0; }

        size_t uniq_cnt = 0;
        for (size_t i = 0; i < pair_cnt; )
        {
            size_t j = i + 1;
            while (j < pair_cnt && cmp_seq_t(&all[i], &all[j]) == 0)
                ++j;
            if (j - i == 1)          /* unique */
                uniq[uniq_cnt++] = all[i];
            i = j;
        }

        /*  Replace the cache with the freshly computed list of unique
         *  sums.  Earlier cache is discarded; it is no longer needed
         *  because all earlier terms are included in the new list.  */
        free(cache);
        cache = uniq;
        cache_len = uniq_cnt;

        /*  Clean up temporary arrays and prepare for the next iteration.  */
        free(all);
        free(pow4);
        B = new_B;
    }

    /*  Return the requested term (1‑based index).  */
    return cache[n - 1];
}
