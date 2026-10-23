/*
 * A104022  Numbers that have more than one way of being written as k^2*j, 0 <
 *          j <= k.
 * https://oeis.org/A104022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th number having more than one representation as
 *            k^2 * j with 0 < j <= k. Computed by enumerating all k^2 * j
 *            values up to a growing bound K, sorting them, and keeping those
 *            occurring at least twice. The list is rebuilt only when a larger
 *            index is requested
 * Range:     u8 none, u16 none, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   compare_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compare_seq_t.h"

/* Global data used to cache computed terms. */
static seq_t *terms = NULL;      /* sorted qualifying numbers */
static size_t terms_len = 0;     /* number of stored terms      */
static idx_t  current_K = 0;     /* current maximum k used      */

/* Build or rebuild the list of qualifying numbers until at least 'need' terms are stored. */
static void build_terms(idx_t need)
{
    /* Already have enough terms? */
    if (terms_len >= need) return;

    /* Start with a moderately sized k and grow until we have enough terms. */
    idx_t K = (current_K == 0) ? 512 : current_K;

    while (true) {
        /* Total number of k,j pairs up to K: K*(K+1)/2.  Safe cast to size_t for allocation. */
        size_t pair_cnt = (size_t)K * (K + 1) / 2;

        /* Allocate buffer for all products. */
        seq_t *all = (seq_t *)malloc(pair_cnt * sizeof(seq_t));
        if (!all) exit(EXIT_FAILURE);               /* out of memory */

        /* Generate all k^2 * j values. */
        size_t pos = 0;
        for (idx_t k = 1; k <= K; ++k) {
            seq_t k2 = (seq_t)k * (seq_t)k;
            for (idx_t j = 1; j <= k; ++j) {
                all[pos++] = k2 * (seq_t)j;
            }
        }

        /* Sort the generated values. */
        qsort(all, pair_cnt, sizeof(seq_t), compare_seq_t);

        /* Count repetitions and keep values occurring at least twice. */
        size_t qual_est = pair_cnt / 2; /* rough upper bound */
        if (qual_est == 0) qual_est = 1;
        seq_t *qual = (seq_t *)malloc(qual_est * sizeof(seq_t));
        if (!qual) exit(EXIT_FAILURE);

        size_t qlen = 0;
        size_t i = 0;
        while (i < pair_cnt) {
            seq_t val = all[i];
            size_t j = i + 1;
            while (j < pair_cnt && all[j] == val) ++j;
            if (j - i >= 2) {                  /* at least two representations */
                if (qlen == qual_est) {        /* grow buffer if needed */
                    qual_est *= 2;
                    qual = (seq_t *)realloc(qual, qual_est * sizeof(seq_t));
                    if (!qual) exit(EXIT_FAILURE);
                }
                qual[qlen++] = val;
            }
            i = j;
        }

        free(all);  /* free product buffer */

        if (qlen >= need) {                     /* we have enough terms */
            free(terms);                        /* discard old list */
            terms = qual;
            terms_len = qlen;
            current_K = K;
            return;
        }

        /* Not enough: discard and try again with larger K. */
        free(qual);
        K *= 2;                                 /* increase bound */
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, safe fallback */
    build_terms(n);               /* ensure at least 'n' terms are available */
    return terms[n - 1];          /* sequence is 0‑based internally */
}
