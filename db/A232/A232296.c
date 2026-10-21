/*
 * A232296  Number of 2Xn 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally or antidiagonally.
 * https://oeis.org/A232296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 6*a(n‑1) - a(n‑2) + 13*a(n‑3) + 2*a(n‑4) * - a(n‑5) +
 *            a(n‑6) - 14*a(n‑7) - 2*a(n‑8) + 5*a(n‑9) * Initial values (n =
 *            1…9): * 1, 15, 97, 587, 3615, 22387, 138505, 856719, 5299373
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n‑th term of OEIS A232296.
 * The function is memoised and handles non‑increasing calls. */
seq_t a(idx_t n)
{
    /* cache[0] is unused; cache[i] holds a(i) for i ≥ 1. */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;     /* highest index currently cached */
    static size_t cap   = 0;    /* allocated size - 1 (max index stored) */

    if (n < 1)
        return (seq_t)0;        /* undefined for indices below the offset */

    /* Grow the cache if necessary. */
    if ((size_t)n > cap) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp)            /* allocation failure */
            return (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Fill cache up to n if required. */
    if (max_n < n) {
        idx_t start = max_n + 1;
        if (start < 1) start = 1;
        for (idx_t i = start; i <= n; ++i) {
            seq_t val;
            switch ((int)i) {
                case 1:  val = (seq_t)1;      break;
                case 2:  val = (seq_t)15;     break;
                case 3:  val = (seq_t)97;     break;
                case 4:  val = (seq_t)587;    break;
                case 5:  val = (seq_t)3615;   break;
                case 6:  val = (seq_t)22387;  break;
                case 7:  val = (seq_t)138505; break;
                case 8:  val = (seq_t)856719; break;
                case 9:  val = (seq_t)5299373;break;
                default:
                {
                    seq_t a1 = cache[i-1];
                    seq_t a2 = cache[i-2];
                    seq_t a3 = cache[i-3];
                    seq_t a4 = cache[i-4];
                    seq_t a5 = cache[i-5];
                    seq_t a6 = cache[i-6];
                    seq_t a7 = cache[i-7];
                    seq_t a8 = cache[i-8];
                    seq_t a9 = cache[i-9];

                    /* Compute according to the recurrence. */
                    val = (seq_t)6 * a1
                          - a2
                          + (seq_t)13 * a3
                          + (seq_t)2  * a4
                          - a5
                          + a6
                          - (seq_t)14 * a7
                          - (seq_t)2  * a8
                          + (seq_t)5  * a9;
                }
            }
            cache[i] = val;
        }
        max_n = n;
    }

    return cache[n];
}
