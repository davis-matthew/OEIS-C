/*
 * A126377  Number of base 23 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑23 n‑digit sequences for which adjacent
 *            digits differ by at most 1. Recurrence: a(0)=1, a(1)=23. For
 *            n≥2, let c_d(k) be the number of length‑k sequences ending with
 *            digit d (0≤d<23). Then for each d:
 *            c_d(k+1)=c_d(k)+c_{d-1}(k)+c_{d+1}(k) (missing terms omitted).
 *            a(n)=∑_{d=0}...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define BASE 23

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* cache of a(0)…a(max_n) */
    static idx_t max_n = 0;              /* highest n cached so far   */
    static seq_t dp_prev[BASE], dp_next[BASE]; /* DP tables */

    /* initial allocation for n = 0 */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 1;                    /* a(0) = 1 */
        max_n = 0;
    }
    if (n == 0) return cache[0];

    /* extend cache if necessary */
    if (n > max_n) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {                /* allocation failed – abort */
            exit(1);
        }
        cache = new_cache;

        for (idx_t cur = max_n + 1; cur <= n; ++cur) {
            if (cur == 1) {               /* Base case: length 1 strings */
                for (idx_t d = 0; d < BASE; ++d) {
                    dp_prev[d] = 1;
                }
                cache[1] = (seq_t)BASE;   /* a(1) = 23 */
            } else {
                /* compute dp_next for length cur from dp_prev (length cur-1) */
                for (idx_t d = 0; d < BASE; ++d) {
                    seq_t val = dp_prev[d];
                    if (d > 0)          val += dp_prev[d - 1];
                    if (d + 1 < BASE)   val += dp_prev[d + 1];
                    dp_next[d] = val;
                }
                /* sum to obtain a(cur) */
                seq_t total = 0;
                for (idx_t d = 0; d < BASE; ++d) {
                    total += dp_next[d];
                }
                cache[(size_t)cur] = total;
                /* prepare for next iteration */
                for (idx_t d = 0; d < BASE; ++d) {
                    dp_prev[d] = dp_next[d];
                }
            }
        }
        max_n = n;
    }

    return cache[(size_t)n];
}
