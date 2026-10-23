/*
 * A169094  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the rational generating function (t^25 +
 *            2*t^24 + … + 2*t + 1) / (1 – 31*t – 31*t^2 – … – 31*t^24 +
 *            496*t^25). It satisfies a(0) = 1, a(n) = 2 + 31 *
 *            (a(0)+…+a(n-1)), for 1 ≤ n ≤ 24, a(n) = 31 * (a(n-1)+…+a(n-24))
 *            – 496*a(n-25), for n ≥ 25
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values: a[i] holds a(i), pref[i] holds sum_{0..i} a(j). */
    static seq_t *a_cache = NULL;
    static seq_t *pref_cache = NULL;
    static size_t cap = 0;         /* largest index currently allocated   */
    static size_t max_n = 0;       /* highest index that has been computed */

    /* Allocate initial space for index 0 */
    if (cap == 0) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        pref_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache || !pref_cache) {
            return 0; /* allocation failed */
        }
        a_cache[0] = 1;
        pref_cache[0] = 1;
        cap = 0;
        max_n = 0;
    }

    /* Expand cache if needed */
    if ((idx_t)max_n < n) {
        size_t new_cap = (cap > 0) ? cap : 1;
        while (new_cap < (size_t)n) {
            new_cap <<= 1;           /* double until enough space */
        }

        seq_t *a_tmp = (seq_t *)realloc(a_cache, (new_cap + 1) * sizeof(seq_t));
        seq_t *pref_tmp = (seq_t *)realloc(pref_cache, (new_cap + 1) * sizeof(seq_t));
        if (!a_tmp || !pref_tmp) {
            free(a_cache);
            free(pref_cache);
            return 0;
        }
        a_cache = a_tmp;
        pref_cache = pref_tmp;
        cap = new_cap;

        /* Compute new entries */
        for (size_t i = max_n + 1; i <= new_cap; ++i) {
            if (i <= 24) { /* first 24 terms after a(0) */
                a_cache[i] = (seq_t)2 + ((seq_t)31 * pref_cache[i - 1]);
                pref_cache[i] = pref_cache[i - 1] + a_cache[i];
            } else { /* i ≥ 25 */
                seq_t window = pref_cache[i - 1] - pref_cache[i - 25];
                a_cache[i] = ((seq_t)31 * window) - ((seq_t)496 * a_cache[i - 25]);
                pref_cache[i] = pref_cache[i - 1] + a_cache[i];
            }
        }
        max_n = new_cap;
    }

    return a_cache[n];
}
