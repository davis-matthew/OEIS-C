/*
 * A170185  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(0) = 1, * a(n) = 2 +
 *            30*Σ_{i=1}^{min(n,38)} a(n‑i) for 1 ≤ n ≤ 38, * a(39) = 1 +
 *            30*Σ_{i=1}^{38} a(39‑i) – 465*a(0), * a(n) = 30*Σ_{i=1}^{38}
 *            a(n‑i) – 465*a(n‑39) for n ≥ 40. * This follows from the
 *            generating function * G(t) = (t^39 + 2t^38 + … + 2t + 1)...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array of computed terms          */
    static size_t cached_len = 0;      /* allocated length of the array    */
    static idx_t  computed   = 0;      /* number of terms computed so far  */
    static seq_t win_sum      = 0;      /* sum of the last 38 terms used in the recurrence */

    /* Allocate or enlarge the cache if necessary. */
    if ((size_t)(n + 1) > cached_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        cache = tmp;
        cached_len = n + 1;
    }

    /* Generate terms up to n, reusing any previously computed values. */
    while (computed <= n) {
        idx_t idx = computed;

        /* Handle a(0) separately. */
        if (idx == 0) {
            cache[0] = 1;
            win_sum = cache[0];
            ++computed;
            continue;
        }

        /* 1 <= idx <= 38 : recurrence with p_n = 2. */
        if (idx <= 38) {
            seq_t val = 2 + 30 * win_sum;
            cache[idx] = val;
            win_sum += val;
            if (idx >= 38) {
                win_sum -= cache[idx - 38];   /* drop the element that is now 38 positions back */
            }
            ++computed;
            continue;
        }

        /* idx == 39 : special recurrence with p_39 = 1. */
        if (idx == 39) {
            seq_t val = 1 + 30 * win_sum - 465 * cache[0];
            cache[39] = val;
            win_sum += val;
            win_sum -= cache[1];            /* window now covers terms 2..39 for the next step */
            ++computed;
            continue;
        }

        /* idx >= 40 : general recurrence. */
        seq_t val = 30 * win_sum - 465 * cache[idx - 39];
        cache[idx] = val;
        win_sum += val;
        win_sum -= cache[idx - 38];            /* slide the window forward by one */
        ++computed;
    }

    return cache[n];
}
