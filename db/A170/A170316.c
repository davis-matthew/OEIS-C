/*
 * A170316  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = for 0 ≤ n < 42: a(n) = coeff(n) + 17 ·
 *            Σ_{k=1}^{min(41,n)} a(n‑k) , where coeff(0)=1 and coeff(n)=2 for
 *            1≤n<42; for n ≥ 42: a(n) = 17 · Σ_{k=1}^{41} a(n‑k) – 153 ·
 *            a(n‑42)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <cstddef>
#endif

seq_t a(idx_t n)
{
    static seq_t *series = NULL;        /* cached values   */
    static idx_t  sz     = 0;           /* number of elems */

    if (n >= sz) {
        idx_t old_sz = sz;
        sz = n + 1;                     /* new required size */

        /* grow the array */
        if (series == NULL) {
#ifdef __cplusplus
            series = new seq_t[sz];
#else
            series = (seq_t*)malloc(sz * sizeof(seq_t));
#endif
        } else {
#ifdef __cplusplus
            seq_t *new_series = new seq_t[sz];
            for (idx_t i = 0; i < old_sz; ++i) new_series[i] = series[i];
            delete [] series;
            series = new_series;
#else
            series = (seq_t*)realloc(series, sz * sizeof(seq_t));
#endif
        }

        /* sliding window sum of the last 41 terms */
        seq_t sum41 = 0;

        /* initialise sum41 with the first old_sz values */
        for (idx_t i = 0; i < old_sz; ++i) {
            sum41 += series[i];
            if (i >= 41) sum41 -= series[i - 41];
        }

        /* compute the remaining terms */
        for (idx_t i = old_sz; i < sz; ++i) {
            seq_t val;
            if (i < 42) {                    /* initial values */
                seq_t coeff = (i == 0 ? (seq_t)1 : (seq_t)2);
                val = coeff + (seq_t)17 * sum41;
            } else {                          /* recurrence for n ≥ 42 */
                val = (seq_t)17 * sum41 - (seq_t)153 * series[i - 42];
            }
            series[i] = val;

            /* update the sliding window for the next iteration */
            sum41 += val;
            if (i >= 41) sum41 -= series[i - 41];
        }
    }

    return series[n];
}
