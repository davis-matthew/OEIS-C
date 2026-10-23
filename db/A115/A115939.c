/*
 * A115939  Numbers k such that the k-th triangular number contains each of
 *          the 10 decimal digits exactly once.
 * https://oeis.org/A115939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   digit_counts, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115939: nth integer k such that its triangular number
   T(k)=k*(k+1)/2 is a 10‑digit number containing each decimal digit
   (0–9) exactly once. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_len.h"
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* static cache of found ks */
    static seq_t *results   = NULL;
    static size_t results_cap   = 0;
    static idx_t  results_count = 0;
    static idx_t  max_k_checked = 0;

    /* return cached result if already available */
    if (results_count >= n)
        return results[n-1];

    /* search forward until the nth term is found or an upper bound is reached */
    for (idx_t k = max_k_checked + 1; ; ++k) {
        max_k_checked = k;

        /* triangular number : T = k*(k+1)/2  (performed in seq_t) */
        seq_t T = ((seq_t)k * ((seq_t)(k + 1))) / 2;

        if (digit_len(T) == 10) {
            unsigned char cnt[10] = {0};
            digit_counts(T, cnt);

            bool ok = true;
            for (int d = 0; d < 10 && ok; ++d)
                if (cnt[d] != 1)
                    ok = false;

            if (ok) {
                /* enlarge cache if necessary */
                if (results_count == results_cap) {
                    size_t new_cap = results_cap == 0 ? 16 : results_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
                    if (!tmp)   /* allocation failure: abort gracefully */
                        return 0;
                    results = tmp;
                    results_cap = new_cap;
                }
                results[results_count++] = k;
                if (results_count >= n)
                    return results[n-1];
            }
        }

        /* The largest known element is k = 138959; an upper bound of 200000
           is sufficient to guarantee that all 82 terms are found.  This
           protects against an infinite loop when n exceeds the available
           number of terms. */
        if (k > 200000)
            break;
    }

    /* n was larger than the number of known terms; return 0 as a sentinel. */
    return 0;
}
